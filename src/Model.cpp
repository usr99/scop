/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:45:16 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/12 19:17:26 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <stdexcept>

#include "Model.hpp"
#include "parser.hpp"
#include "ftGraphics.hpp"

Model::Model(const std::string& path) : _M_VerticesCount(0), _M_ModelMatrix(1.0f)
{
	/* Check file extension */
	size_t extensionIndex = path.find(".obj", path.length() - 4);
	if (extensionIndex == std::string::npos)
		throw std::invalid_argument("Bad file extension, must be .obj");

	/* Open object file */
	std::ifstream file(path);
	if (!file)
		throw std::invalid_argument(path + " not found");

	ObjectInfo obj = loadObjectFile(file);
	file.close();
	
	std::vector<float> vxBuffer;
	std::vector<unsigned int> idxBuffer;
	
	/* Allocate enough memory to contain all vertices */
	for (auto face : obj.faces)
		_M_VerticesCount += face.size() / 3;
	vxBuffer.reserve(_M_VerticesCount * 9);
	idxBuffer.reserve(_M_VerticesCount);

	/*
	** Convert obj formatted data
	** In a .obj file, the same coordinates can be used multiple times with different texture coordinates and normals
	** In a OpenGL VBO, each vertex is a unique combination of these attributes
	** Thus we have to copy some of the .obj face to create our vertices
	*/
	unsigned int index = 0;
	for (auto face : obj.faces)
	{
		bool isTriangle = (face.size() == 3 * 3);

		for (unsigned int i = 0; i < face.size(); i += 3)
		{
			/* Create a new vertex from the indices in the face component of the object file */
			_insertVertexAttribute(vxBuffer, obj.vertices, face[i]);
			_insertVertexAttribute(vxBuffer, obj.textures, face[i + 1]);
			_insertVertexAttribute(vxBuffer, obj.normals, face[i + 2]);

			if (isTriangle)
				idxBuffer.push_back(index);
			index++;
		}

		// if (!isTriangle)
	}

	/* Create vertex buffer object from converted data */
	GLCall(glGenBuffers(1, &_M_VertexBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _M_VertexBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vxBuffer.size() * sizeof(float), vxBuffer.data(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &_M_IndexBuffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _M_IndexBuffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxBuffer.size() * sizeof(unsigned int), idxBuffer.data(), GL_STATIC_DRAW));

	/*
	** Set vertex layout
	** each vertex follows this pattern -> Coordinate|Texture|Normal
	*/
	size_t offset = 0;
	for (int i = 0; i < 4; i++)
	{
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (const char*)(offset)));
		offset += + 3 * sizeof(float);
	}
}

void
Model::render() const
{
	GLCall(glDrawElements(GL_TRIANGLES, _M_VerticesCount, GL_UNSIGNED_INT, 0));
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
Model::_insertVertexAttribute(std::vector<float>& buffer, std::vector<float>& from, int index)
{
	if (index != 0)
	{
		index--;
		buffer.insert(buffer.end(), from.begin() + index * 3, from.begin() + (index + 1) * 3);
	}
	else
		buffer.insert(buffer.end(), { 0.f, 0.f, 0.f });
}
