/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:45:16 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/01 17:15:42 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <map>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Model.hpp"
#include "debug.hpp"

Model::Model(const std::string& path)
	:	_M_ObjectInfo(path),
		_M_RotationAngle(0.f), _M_RenderingMode(GL_TRIANGLES), _M_PointSize(1)
{
	/* Initialize RNG which is needed to generate faces colors */
	std::srand(time(nullptr));

	std::vector<float> vxBuffer(_M_ObjectInfo.vertexCount * 12, 0.f);
	std::vector<unsigned int> idxBuffer;
	idxBuffer.reserve(_M_ObjectInfo.vertexCount);
	_M_Palette.reserve(_M_ObjectInfo.vertexCount * 3);

	/*
	** Compute the translation vector to place the object at the center of the world space
	** it also stores the most extreme coordinate (negative or not) to scale the object
	*/
	glm::vec3 mean;
	float biggest = 0;
	for (int i = 0; i < 3; i++)
	{
		mean[i] = -(_M_ObjectInfo.max[i] + _M_ObjectInfo.min[i]) / 2;
		biggest = std::max(std::max(std::abs(_M_ObjectInfo.max[i]), std::abs(_M_ObjectInfo.min[i])), biggest);
	}
	float scalingFactor = 2 / biggest;
	for (auto vertex = _M_ObjectInfo.vertices.begin(); vertex != _M_ObjectInfo.vertices.end(); vertex++)
		*vertex = (*vertex + mean) * scalingFactor; // translate and scale the vertex coordinate to fit the range [-2;2]

	/*
	** Convert obj formatted data
	** In a .obj file, the same coordinates can be used multiple times with different texture coordinates and normals
	** In a OpenGL VBO, each vertex is a unique combination of these attributes
	** Thus we have to copy some of the faces to create our vertices
	*/
	unsigned int vertexIndex = 0;
	unsigned int faceIndex = 0;
	unsigned int lastIndex = 0;

	/* Generate texture coordinates if not provided in the file */
	if (_M_ObjectInfo.textures.size() == 0)
	{
		glm::vec3 coordinates[4] = {
			glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(1.f, 1.f, 0.f),
			glm::vec3(0.f, 1.f, 0.f),
			glm::vec3(0.f, 0.f, 0.)
		};
		_M_ObjectInfo.textures.insert(_M_ObjectInfo.textures.end(), coordinates, coordinates + 4);
	}

	for (auto group = _M_ObjectInfo.groups.begin(); group != _M_ObjectInfo.groups.end(); group++)
	{
		/* Generate face normals for the polygons missing them */
		std::map<unsigned int, std::list<unsigned int*>> storage;
		for (auto& polygon : group->polygons)
		{
			/* Check that every vertex has a normal vector */
			bool isMissingNormalVector = true;
			for (size_t i = 0; i < polygon.size(); i++)
			{
				if (polygon[i].normal() != 0)
					isMissingNormalVector = false;
			}

			/*
			** If that's not the case generate it
			** 3 vertices make a plane, compute the perpendicular vector of this plane
			** using the cross product
			*/
			if (isMissingNormalVector)
			{
				glm::vec3 points[3];
				for (int i = 0; i < 3; i++)
					std::copy_n(&_M_ObjectInfo.vertices[(polygon[i].position() - 1)], 1, &points[i]);
				glm::vec3 normalVector = glm::normalize(glm::cross(points[0] - points[1], points[0] - points[2]));
				_M_ObjectInfo.normals.push_back(normalVector);
			
				for (auto& vertex : polygon)
				{
					vertex.normal() = _M_ObjectInfo.normals.size(); // set the normal index to the newly generated vector
					if (group->enabled) // save a pointer to the index to compute smooth shading later
						storage[vertex.position()].push_back(vertex.data() + 2);
				}
			}
		}

		if (group->enabled) // smooth shading enabled
		{
			for (auto vertex = storage.begin(); vertex != storage.end(); vertex++)
			{
				/* Compute the average of all the face normals the vertex belong to */
				glm::vec3 smoothed(0.f);
				for (auto index = vertex->second.begin(); index != vertex->second.end(); index++)
					smoothed += _M_ObjectInfo.normals[**index - 1];
				smoothed = glm::normalize(smoothed);

				/* Set this new normal vector for the vertex */
				_M_ObjectInfo.normals.push_back(smoothed);
				for (auto index = vertex->second.begin(); index != vertex->second.end(); index++)
					**index = _M_ObjectInfo.normals.size();
			}
		}

		for (auto polygon : group->polygons)
		{
			bool isTriangle = (polygon.size() == 3);
			const float greyColor = (static_cast<float>(std::rand()) / RAND_MAX) * 0.4f + 0.1f;
			unsigned int textureIndex = 0;

			for (unsigned int i = 0; i < polygon.size(); i++)
			{
				/* same for texture coordinates */
				textureIndex = textureIndex % 4 + 1;
				if (polygon[i].uv() == 0)
					polygon[i].uv() = textureIndex;

				/* Create a new vertex from the indices in the face component of the object file */
				_insertVertexAttribute(vxBuffer, (_M_ObjectInfo.vertexCount * 0 + vertexIndex) * 3, _M_ObjectInfo.vertices, polygon[i].position());
				_insertVertexAttribute(vxBuffer, (_M_ObjectInfo.vertexCount * 1 + vertexIndex) * 3, _M_ObjectInfo.textures, polygon[i].uv());
				_insertVertexAttribute(vxBuffer, (_M_ObjectInfo.vertexCount * 2 + vertexIndex) * 3, _M_ObjectInfo.normals, polygon[i].normal());
				
				_M_Palette.push(greyColor);
				if (isTriangle)
					idxBuffer.push_back(vertexIndex);

				vertexIndex++;
			}

			/*
			** With non-triangle polygons, it is needed to divide them into triangles
			** faces indices are used to create triangles inside the polygon
			** it is done in a way similar to the GL_TRIANGLE_STRIP rendering mode
			*/
			if (!isTriangle)
			{
				std::deque<unsigned int> indices({1, 0, 2}); // first triangle is 1 0 2
				const int nbTriangles = polygon.size() - 2;
				int direction = -1;

				for (int i = 0; i < nbTriangles; i++)
				{
					/*
					** Insert indices into the buffer
					** each time lastIndex is added because until now we worked with indices starting from 0
					** but we need to take into account previous faces
					*/
					std::for_each(indices.begin(), indices.end(), [&](unsigned int value)
								  { idxBuffer.push_back(value + lastIndex); });
					indices.pop_front(); // the last two indices are kept for the next triangle

					/* The third index is equal to the first one +/-1 */
					int nextIndex = indices[0] + direction;
					if (nextIndex < 0) // index == 0 - 1
						nextIndex = polygon.size() - 1;

					indices.push_back(nextIndex);
					direction *= -1;
				}
			}

			faceIndex++;
			lastIndex += polygon.size();
		}
	}

	/* Create VBOs from converted data */
	GLCall(glGenBuffers(1, &_M_VertexBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _M_VertexBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vxBuffer.size() * sizeof(float), vxBuffer.data(), GL_STATIC_DRAW));

	_M_IndicesCount = idxBuffer.size();
	GLCall(glGenBuffers(1, &_M_IndexBuffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _M_IndexBuffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _M_IndicesCount * sizeof(unsigned int), idxBuffer.data(), GL_STATIC_DRAW));

	/*
	** Set vertex layout
	** the attributes are tightly packed in the buffer, as specified by the stride of 0
	** respectively vertex coordinates, textures coordinates, normals and colors
	*/
	size_t offset = 0;
	for (int i = 0; i < 5; i++)
	{
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, (const char*)(offset)));
		offset += _M_ObjectInfo.vertexCount * 3 * sizeof(float);
	}

	_M_ObjectInfo.debug();
}

void
Model::render()
{
	_M_Palette.update();
	if (_M_RenderingMode == GL_POINTS)
		GLCall(glPointSize(_M_PointSize));
	GLCall(glDrawElements(_M_RenderingMode, _M_IndicesCount, GL_UNSIGNED_INT, 0));
}

void
Model::showSettingsPanel()
{
	_M_Palette.showSettings();

	ImGui::RadioButton("Triangles", &_M_RenderingMode, GL_TRIANGLES);
	ImGui::RadioButton("Wireframe", &_M_RenderingMode, GL_LINE_STRIP);
	ImGui::RadioButton("Dots", &_M_RenderingMode, GL_POINTS);
	ImGui::BeginDisabled(_M_RenderingMode != GL_POINTS);
		ImGui::SliderInt("size", &_M_PointSize, 1, 10);
	ImGui::EndDisabled();
}

void
Model::rotate(float angle)
{
	_M_RotationAngle += glm::radians(angle);
	if (_M_RotationAngle > 2 * M_PI)
		_M_RotationAngle = 0.f;
}

glm::mat4
Model::getMatrix() const
{
	glm::mat4 matrix(1.f);
	const glm::vec3 axis(0.f, 1.f, 0.f);

	if (_M_RotationAngle != 0.f)
		matrix = glm::rotate(matrix, _M_RotationAngle, axis);
	return matrix;
}

void
Model::_insertVertexAttribute(
	std::vector<float>& buffer,		// vertex buffer where the data is inserted
	unsigned int offset, 			// starting position of the inserted data
	std::vector<glm::vec3>& from,	// source array from which the vertices are copied
	unsigned int index				// starting position from where to copy
) {
	if (index > from.size())
		throw std::runtime_error("Invalid face index found in .obj file");

	if (index != 0)
	{
		index--;
		std::copy_n(reinterpret_cast<float*>(from.data() + index), 3, buffer.begin() + offset);
	}
}
