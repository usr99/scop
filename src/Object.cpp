/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 05:55:20 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/30 15:51:24 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include "Object.hpp"

Object::Object(const std::string& filename)
	: groups(1, ShadingGroup(false)), vertexCount(0)
{
	/* Check file extension */
	size_t extensionIndex = filename.find(".obj", filename.length() - 4);
	if (extensionIndex == std::string::npos)
		throw std::invalid_argument("Bad file extension, must be .obj");

	std::ifstream file(filename);
	if (file)
	{
		std::string line;
		int lineIndex = 1;
		while (std::getline(file, line))
		{
			try
			{
				switch (line.front())
				{
					case 'v':
						switch (line[1])
						{
							case ' ':
								(this->*_ParserFunctions[Attribute::VERTEX])(line, _split(line));
								break;
							case 't':
								(this->*_ParserFunctions[Attribute::TEXTURE])(line, _split(line));
								break;
							case 'n':
								(this->*_ParserFunctions[Attribute::NORMAL])(line, _split(line));
								break;
						}
						break;
					case 'f':
						(this->*_ParserFunctions[Attribute::POLYGON])(line, _split(line));
						break;
					case 's':
						int value = std::atoi(line.c_str() + 2);
						bool enable = (value > 0 || line.compare(2, std::string::npos, "on") == 0);
						groups.emplace_back(enable);
						break;
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
Object::debug() const
{
    for (auto it = vertices.begin(); it != vertices.end(); it++)
		std::cout << "v " << it->x << ' ' << it->y << ' ' << it->z << '\n';
	std::cout << '\n';

    for (auto it = textures.begin(); it != textures.end(); it++)
		std::cout << "vt " << it->x << ' ' << it->y << ' ' << it->z << '\n';
	std::cout << '\n';

    for (auto it = normals.begin(); it != normals.end(); it++)
		std::cout << "vn " << it->x << ' ' << it->y << ' ' << it->z << '\n';
	std::cout << '\n';

	for (auto group = groups.begin(); group != groups.end(); group++)
	{
		std::cout << std::boolalpha << group->enabled << '\n';
		for (auto polygon = group->polygons.begin(); polygon != group->polygons.end(); polygon++)
		{
			std::cout << "f ";
			for (auto vertex = polygon->begin(); vertex != polygon->end(); vertex++)
				std::cout << vertex->at(0) << '/' << vertex->at(1) << '/' << vertex->at(2) << ' ';
			std::cout << '\n';
		}
		std::cout << '\n';
	}
	std::cout << "max: " << max << "\nmin: " << min << '\n';
}

void
Object::_parseVertex(const std::string& line, const std::vector<indices_t>& indices)
{
	if (indices.size() != 4 && indices.size() != 5)
		throw std::invalid_argument("invalid number of coordinates");

	vertices.emplace_back();
	float w = (indices.size() == 5) ? std::atof(line.c_str() + indices[4].first) : 1.f;
	for (int i = 1; i < 4; i++)
	{
		const float value = std::atof(line.c_str() + indices[i].first) / w;
		const int coordinate = i - 1;

		vertices.back()[coordinate] = value;
		max[coordinate] = std::max(value, max[coordinate]);
		min[coordinate] = std::min(value, min[coordinate]);
	}
}

void
Object::_parseUV(const std::string& line, const std::vector<indices_t>& indices)
{
    if (indices.size() < 2 || indices.size() > 4)
		throw std::invalid_argument("invalid number of coordinates");

    textures.emplace_back();
	unsigned int i = 1;
	for (; i < indices.size(); i++)
		textures.back()[i - 1] = std::atof(line.c_str() + indices[i].first);
	for (; i < 4; i++)
		textures.back()[i - 1] = 0.f;
}

void
Object::_parseNormal(const std::string &line, const std::vector<indices_t> &indices)
{
	if (indices.size() != 4)
		throw std::invalid_argument("invalid number of coordinates");

	normals.emplace_back();
	for (int i = 1; i < 4; i++)
		normals.back()[i - 1] = std::atof(line.c_str() + indices[i].first);
}

void
Object::_parsePolygon(const std::string &line, const std::vector<indices_t> &indices)
{
	if (indices.size() < 4)
		throw std::invalid_argument("a polygon must have at least 3 vertices");

	groups.back().polygons.emplace_back();
	for (auto it = indices.begin() + 1; it != indices.end(); it++)
	{
		const std::string block(line, it->first, it->second);
		auto elements = _split(block, '/', true);

		groups.back().polygons.back().emplace_back();
		for (unsigned int i = 0; i < 3; i++)
		{
			if (i < elements.size())
			{
				if (elements[i].second != 0)
				{
					int vertexIndex = std::atoi(block.c_str() + elements[i].first);
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

std::vector<Object::indices_t>
Object::_split(const std::string& str, char delim, bool allowNullStrings)
{
	std::vector<indices_t> indices;

	size_t start = 0;
	while (start < str.length())
	{
		size_t end = str.find(delim, start);
		if (end - start > 0 || allowNullStrings)
			indices.push_back(std::make_pair(start, end - start));
		start = (end != std::string::npos) ? end + 1 : std::string::npos;
	}
	return indices;	
}

ShadingGroup::ShadingGroup(bool smoothShadingOn) : enabled(smoothShadingOn) {}

Vertex::Vertex() : std::array<unsigned int, 3>() {}

unsigned int&
Vertex::position()
{
	return operator[](0);
}

unsigned int&
Vertex::uv()
{
	return operator[](1);
}

unsigned int&
Vertex::normal()
{
	return operator[](2);
}
