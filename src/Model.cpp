/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:45:16 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/20 17:49:44 by mamartin         ###   ########.fr       */
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
	float scale_factor = 2 / biggest;
	int i = 0;
	for (auto vertex = obj.vertices.begin(); vertex != obj.vertices.end(); vertex++)
	{
		*vertex += mean[i];			// translate
		*vertex *= scale_factor;	// scale the vertex coordinate to fit the range [-2;2]
		i = (i + 1) % 3;
	}

	/*
	** Convert obj formatted data
	** In a .obj file, the same coordinates can be used multiple times with different texture coordinates and normals
	** In a OpenGL VBO, each vertex is a unique combination of these attributes
	** Thus we have to copy some of the .obj face to create our vertices
	*/
	unsigned int vertexIndex = 0;
	unsigned int faceIndex = 0;

	for (auto face : obj.faces)
	{
		bool isTriangle = (face.size() == 3 * 3);

		for (unsigned int i = 0; i < face.size(); i += 3)
		{
			/* Create a new vertex from the indices in the face component of the object file */
			_insertVertexAttribute(vxBuffer, (_M_VerticesCount * 0 + vertexIndex) * 3, obj.vertices, face[i]);
			_insertVertexAttribute(vxBuffer, (_M_VerticesCount * 1 + vertexIndex) * 3, obj.textures, face[i + 1]);
			_insertVertexAttribute(vxBuffer, (_M_VerticesCount * 2 + vertexIndex) * 3, obj.normals, face[i + 2]);
			if (isTriangle)
				idxBuffer.push_back(vertexIndex);
			vertexIndex++;
		}
		faceIndex++;

		// if (!isTriangle)
	}

	/* Create VBOs from converted data */
	GLCall(glGenBuffers(1, &_M_VertexBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _M_VertexBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vxBuffer.size() * sizeof(float), vxBuffer.data(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &_M_IndexBuffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _M_IndexBuffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxBuffer.size() * sizeof(unsigned int), idxBuffer.data(), GL_STATIC_DRAW));

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
	GLCall(glDrawElements(GL_TRIANGLES, _M_VerticesCount, GL_UNSIGNED_INT, 0));
}

void
Model::showSettingsPanel()
{
	ImGui::Begin("Color Settings");
	ImGui::Checkbox("Add shades ?", _M_ColorMode + SHADES_MODE);
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

	for (size_t i = 0; i < palette.capacity(); i += 3)
	{
		std::array<float, 3> rgb = {0.f};
		generateColorFunc(rgb);

		if (_M_ColorMode[GRADIENT_MODE]) // only write the color for one vertex
			palette.insert(palette.end(), rgb.begin(), rgb.end());
		else // write the color for the whole triangle
		{
			for (int j = 0; j < 3; j++)
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
		palette.data())
	);
}
