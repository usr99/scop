/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 05:55:20 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/03 10:06:56 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>

#include "Object.hpp"
#include "parser.hpp"

Vertex::Vertex() : std::array<unsigned int, 3>() {}

unsigned int&
Vertex::position() { return operator[](0); }

unsigned int&
Vertex::uv() { return operator[](1); }

unsigned int&
Vertex::normal() { return operator[](2); }

Polygon::Polygon(const Material* materialPtr) : mtl(materialPtr) {}

ShadingGroup::ShadingGroup(bool smoothShadingOn) : enabled(smoothShadingOn) {}

Object::Object(const std::string& filename)
	: groups(1, ShadingGroup(false)), vertexCount(0), _M_currentMaterial(nullptr)
{
	checkFileExtension(filename, ".obj");
	std::ifstream file(filename);
	if (file)
	{
		std::string line;
		int lineIndex = 1;
		while (std::getline(file, line))
		{
			try
			{
				auto elements = split(line);
				if (elements.size())
				{
					auto statement = elements.front();
					switch (statement.front())
					{
						case 'm':
							(this->*_ParserFunctions[Attribute::MATERIAL_LIB])(elements);
							break;
						case 'u':
							(this->*_ParserFunctions[Attribute::MATERIAL])(elements);
							break;					
						case 'v':
							switch (statement[1])
							{
								case '\0':
									(this->*_ParserFunctions[Attribute::VERTEX])(elements);
									break;
								case 't':
									(this->*_ParserFunctions[Attribute::TEXTURE])(elements);
									break;
								case 'n':
									(this->*_ParserFunctions[Attribute::NORMAL])(elements);
									break;
							}
							break;
						case 'f':
							(this->*_ParserFunctions[Attribute::POLYGON])(elements);
							break;
						case 's':
							int value = std::atoi(line.c_str() + 2);
							bool enable = (value > 0);
							groups.emplace_back(enable);
							break;
					}
				}
			}
			catch (const std::exception& e)
			{
				throw std::invalid_argument("Bad syntax at line " + std::to_string(lineIndex) + ": " + e.what());
			}
			lineIndex++;
		}
	}
	else
		throw std::runtime_error("Failed to open " + filename);
}

void
Object::_parseVertex(const LineElements& splitted)
{
	if (splitted.size() != 4 && splitted.size() != 5)
		throw std::invalid_argument("invalid number of coordinates");

	vertices.emplace_back();
	float w = (splitted.size() == 5) ? std::stof(splitted[4]) : 1.f;
	for (int i = 1; i < 4; i++)
	{
		const float value = std::stof(splitted[i]) / w;
		const int coordinate = i - 1;

		vertices.back()[coordinate] = value;
		max[coordinate] = std::max(value, max[coordinate]);
		min[coordinate] = std::min(value, min[coordinate]);
	}
}

void
Object::_parseUV(const LineElements& splitted)
{
    if (splitted.size() < 2 || splitted.size() > 4)
		throw std::invalid_argument("invalid number of coordinates");

    textures.emplace_back();
	unsigned int i = 1;
	for (; i < splitted.size(); i++)
		textures.back()[i - 1] = std::stof(splitted[i]);
	for (; i < 4; i++)
		textures.back()[i - 1] = 0.f;
}

void
Object::_parseNormal(const LineElements& splitted)
{
	if (splitted.size() != 4)
		throw std::invalid_argument("invalid number of coordinates");

	normals.emplace_back();
	for (int i = 1; i < 4; i++)
		normals.back()[i - 1] = std::stof(splitted[i]);
}

void
Object::_parsePolygon(const LineElements& splitted)
{
	if (splitted.size() < 4)
		throw std::invalid_argument("a polygon must have at least 3 vertices");

	groups.back().polygons.emplace_back(_M_currentMaterial);
	for (auto block = splitted.begin() + 1; block != splitted.end(); block++)
	{
		auto elements = split(*block, '/', true);

		vertexCount++;
		groups.back().polygons.back().emplace_back();
		for (unsigned int i = 0; i < 3; i++)
		{
			if (i < elements.size())
			{
				if (elements[i].length())
				{
					int vertexIndex = std::stoi(elements[i]);
					if (vertexIndex < 1)
						throw std::invalid_argument("indices must be strictly greater than 0");
					groups.back().polygons.back().back()[i] = vertexIndex;
				}
				else if (i == 0)
					throw std::invalid_argument("vertex index cannot be omitted");
				else
					groups.back().polygons.back().back()[i] = 0;
			}
			else
				groups.back().polygons.back().back()[i] = 0;
		}
	}
}

void
Object::_parseMaterialLibrary(const LineElements& splitted)
{
	if (splitted[0] == "mtllib")
	{
		if (splitted.size() < 2)
			throw std::invalid_argument("missing material library name");
		for (size_t i = 1; i < splitted.size(); i++)
			loadMaterialLibrary(splitted[i], materials);
	}
}

void
Object::_parseMaterial(const LineElements& splitted)
{
	if (splitted[0] == "usemtl")
	{
		if (splitted.size() < 2)
			throw std::invalid_argument("missing material name");
		else if (splitted.size() > 2)
			throw std::invalid_argument("only one material name must be specified");
		
		auto mtl = materials.find(splitted[1]);
		if (mtl != materials.end())
			_M_currentMaterial = &mtl->second;
		else
			_M_currentMaterial = nullptr;
	}
}
