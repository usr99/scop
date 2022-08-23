/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:45:16 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/22 22:33:49 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <ctime>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Model.hpp"
#include "parser.hpp"
#include "debug.hpp"

Model::Model(const std::string& path)
	: _M_VerticesCount(0), _M_ModelMatrix(1.0f),
		_M_ColorMode{ false }, _M_ColorModeMask(-1)
{
	/* Check file extension */
	size_t extensionIndex = path.find(".obj", path.length() - 4);
	if (extensionIndex == std::string::npos)
		throw std::invalid_argument("Bad file extension, must be .obj");

	/* Load 3D model */
	std::ifstream file(path);
	if (!file)
		throw std::invalid_argument(path + " not found");
	ObjectInfo obj = loadObjectFile(file);
	file.close();

	/* Allocate enough memory to contain all vertices */
	for (auto face : obj.faces)
		_M_VerticesCount += face.size() / 3;
	std::vector<float> vxBuffer(_M_VerticesCount * 12, 0.f);
	std::vector<unsigned int> idxBuffer;
	idxBuffer.reserve(_M_VerticesCount);

	/*
	** Compute the translation vector to place the object at the center of the world space
	** it also stores the most extreme coordinate (negative or not) to scale the object
	*/
	glm::vec3 mean;
	float biggest = 0;
	for (int i = 0; i < 3; i++)
	{
		mean[i] = -(obj.max[i] + obj.min[i]) / 2;
		biggest = std::max(std::max(std::abs(obj.max[i]), std::abs(obj.min[i])), biggest);
	}
	float scalingFactor = 2 / biggest;
	int coordIndex = 0;
	for (auto vertex = obj.vertices.begin(); vertex != obj.vertices.end(); vertex++)
	{
		*vertex += mean[coordIndex];		// translate
		*vertex *= scalingFactor;			// scale the vertex coordinate to fit the range [-2;2]
		coordIndex = (coordIndex + 1) % 3;	// switch between x, y and z coordinates
	}

	/*
	** Convert obj formatted data
	** In a .obj file, the same coordinates can be used multiple times with different texture coordinates and normals
	** In a OpenGL VBO, each vertex is a unique combination of these attributes
	** Thus we have to copy some of the faces to create our vertices
	*/
	unsigned int vertexIndex = 0;
	unsigned int faceIndex = 0;
	unsigned int lastIndex = 0;

	for (auto face : obj.faces)
	{
		bool isTriangle = (face.size() == 3 * 3);

		/* Check that every vertex has a normal vector */
		bool isMissingNormalVector = true;
		for (size_t normalIndex = 2; normalIndex < face.size(); normalIndex += 3)
		{
			if (face[normalIndex] != 0)
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
				std::copy_n(&obj.vertices[(face[i * 3] - 1) * 3], 3, &points[i][0]);
			glm::vec3 normalVector = glm::cross(points[0] - points[1], points[0] - points[2]);
			obj.normals.insert(obj.normals.end(), &normalVector.x, &normalVector.z + 1);
		}

		for (unsigned int i = 0; i < face.size(); i += 3)
		{
			/* Set the normal vector of the vertex if not already present */
			if (face[i + 2] == 0)
				face[i + 2] = obj.normals.size() / 3;
			
			/* Create a new vertex from the indices in the face component of the object file */
			_insertVertexAttribute(vxBuffer, (_M_VerticesCount * 0 + vertexIndex) * 3, obj.vertices, face[i]);
			_insertVertexAttribute(vxBuffer, (_M_VerticesCount * 1 + vertexIndex) * 3, obj.textures, face[i + 1]);
			_insertVertexAttribute(vxBuffer, (_M_VerticesCount * 2 + vertexIndex) * 3, obj.normals, face[i + 2]);
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
			std::deque<unsigned int> indices({ 1, 0, 2 }); // first triangle is 1 0 2
			const int nbTriangles = face.size() / 3 - 2;
			int direction = -1;

			for (int i = 0; i < nbTriangles; i++)
			{
				/*
				** Insert indices into the buffer
				** each time lastIndex is added because until now we worked with indices starting from 0
				** but we need to take into account previous faces
				*/
				std::for_each(indices.begin(), indices.end(), [&](unsigned int value) {
					idxBuffer.push_back(value + lastIndex);
				});
				indices.pop_front(); // the last two indices are kept for the next triangle

				/* The third index is equal to the first one +/-1 */
				int nextIndex = indices[0] + direction;
				if (nextIndex < 0) // index == 0 - 1
					nextIndex = face.size() / 3 - 1;

				indices.push_back(nextIndex);
				direction *= -1;
			}
		}

		faceIndex++;
		lastIndex += face.size() / 3;
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
		offset += _M_VerticesCount * 3 * sizeof(float);
	}

	/* Initialize RNG which is needed to generate faces colors */
	std::srand(time(nullptr));
}

void
Model::render()
{
	/* Generate a new array of color attributes for the vertices if the settings have changed */
	char colorMask = _getCurrentColorModeMask();
	if (colorMask != _M_ColorModeMask)
	{
		_generateColorPalette();
		_M_ColorModeMask = colorMask;
	}
	GLCall(glDrawElements(GL_TRIANGLES, _M_IndicesCount, GL_UNSIGNED_INT, 0));
}

void
Model::showSettingsPanel()
{
	ImGui::Begin("Color Settings");
	ImGui::Checkbox("Show triangles ?", _M_ColorMode + SHADES_MODE);
	ImGui::BeginDisabled(!_M_ColorMode[SHADES_MODE]);
		ImGui::Checkbox("Shuffle colors ?", _M_ColorMode + RANDOM_MODE);
		ImGui::BeginDisabled(!_M_ColorMode[RANDOM_MODE]);
			ImGui::Checkbox("RGB ?", _M_ColorMode + COLOR_MODE);
			ImGui::Checkbox("Gradient effect ?", _M_ColorMode + GRADIENT_MODE);
		ImGui::EndDisabled();
	ImGui::EndDisabled();
	ImGui::End();
}

void
Model::translate(const glm::vec3& direction)
{
	_M_ModelMatrix = glm::translate(_M_ModelMatrix, direction);
}

void
Model::rotate(float angle, const glm::vec3& axis)
{
	_M_ModelMatrix = glm::rotate(_M_ModelMatrix, glm::radians(angle), axis);
}

void
Model::scale(float factor)
{
	_M_ModelMatrix = glm::scale(_M_ModelMatrix, glm::vec3(factor, factor, factor));
}

void
Model::_insertVertexAttribute(
	std::vector<float>& buffer,	// vertex buffer where the data is inserted
	int offset, 				// starting position of the inserted data
	std::vector<float>& from,	// source array from which the vertices are copied
	int index					// starting position from where to copy
) {
	if (index != 0)
	{
		index--;
		std::copy_n(from.begin() + index * 3, 3, buffer.begin() + offset);
	}
}

char
Model::_getCurrentColorModeMask() const
{
	char mask = 0;
	for (int i = 0; i < NB_COLOR_SETTINGS; i++)
		mask |= _M_ColorMode[i] << i;
	return mask;
}

void
Model::_generateColorPalette()
{
	std::vector<float> palette;
	std::function<void(std::array<float, 3> &)> generateColorFunc;
	palette.reserve(_M_VerticesCount * 3);

	if (_M_ColorMode[SHADES_MODE])
	{
		if (_M_ColorMode[RANDOM_MODE])
		{
			if (_M_ColorMode[COLOR_MODE])
			{
				// red and green components are random, blue is 1
				generateColorFunc = [](std::array<float, 3> &rgb)
				{
					// rgb[2] = 1.f;
					for (int j = 0; j < 3; j++)
						rgb[j] = static_cast<float>(std::rand()) / RAND_MAX;
				};
			}
			else
			{
				// random shade of grey
				generateColorFunc = [](std::array<float, 3> &rgb)
				{
					rgb.fill(static_cast<float>(std::rand()) / RAND_MAX);
				};
			}
		}
		else
		{
			// starting from black, slowly increment until it is white
			float colorStep = (_M_ColorMode[GRADIENT_MODE] ? 1.f : 3.f) / static_cast<float>(_M_VerticesCount);
			float currentColor = colorStep;			
			generateColorFunc = [&currentColor, colorStep](std::array<float, 3> &rgb)
			{
				rgb.fill(currentColor);
				currentColor += colorStep;
			};
		}
	}
	else
	{
		// only grey
		generateColorFunc = [](std::array<float, 3> &rgb)
		{
			rgb.fill(0.5f);
		};
	}

	const size_t capacity = palette.capacity();
	for (size_t i = 0; i < capacity; i += 3)
	{
		std::array<float, 3> rgb = {0.f};
		generateColorFunc(rgb);

		if (_M_ColorMode[GRADIENT_MODE]) // only write the color for one vertex
			palette.insert(palette.end(), rgb.begin(), rgb.end());
		else // write the color for the whole triangle
		{
			for (int j = 0; j < 3 && palette.size() <= capacity; j++)
				palette.insert(palette.end(), rgb.begin(), rgb.end());
			i += 6;
		}
	}

	/*
	** Copy the new array to GPU memory for later rendering
	** only the last quarter of the buffer is written
	*/
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _M_VertexBuffer));
	GLCall(glBufferSubData(
		GL_ARRAY_BUFFER,
		sizeof(float) * _M_VerticesCount * 3 * 3,
		sizeof(float) * _M_VerticesCount * 3,
		palette.data()
	));
}
