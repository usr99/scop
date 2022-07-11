/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 05:55:20 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/10 20:00:45 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include "parser.hpp"

ObjectInfo loadObjectFile(std::ifstream& file)
{
    ObjectInfo  obj;
	std::string line;
	int         lineIndex = 1;
	
	while (file.good())
	{
		std::getline(file, line);
		auto components = split(line);

		if (components.size())
		{
			if (components[0] == "v")
 				parseVertex(obj, components, lineIndex);
			else if (components[0] == "vt")
 				parseTextureCoordinates(obj, components, lineIndex);
			else if (components[0] == "vn")
 				parseNormalVector(obj, components, lineIndex);
			else if (components[0] == "f")
 				parseFace(obj, components, lineIndex);
		}
		lineIndex++;
	}

	/* Left the loop before the file was fully read */
	if (file.bad())
		throw std::runtime_error("Fatal error occured while reading your file");
    return obj;
}

void parseVertex(ObjectInfo& obj, const SplitArray& values, int line)
{
	if (values.size() < 4 || values.size() > 5)
		throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": invalid number of coordinates");

    // Get w coordinate or 1 if none is provided
    float w = (values.size() == 5) ? std::stof(values[4]) : 1.0f;

    obj.vertices.reserve(obj.vertices.capacity() + 3);
    for (int i = 1 ; i < 4; i++)
        obj.vertices.push_back(std::stof(values[i]) / w);
}

void parseTextureCoordinates(ObjectInfo& obj, const SplitArray& values, int line)
{
    if (values.size() < 2 || values.size() > 4)
		throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": invalid number of coordinates");

    size_t last = obj.textures.size();
    obj.textures.resize(last + 3);
	for (unsigned int i = 1; i < values.size(); i++)
		obj.textures[last + i - 1] = std::stof(values[i]);
}

void parseNormalVector(ObjectInfo& obj, const SplitArray& values, int line)
{
	if (values.size() != 4)
		throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": invalid number of coordinates");

    obj.normals.reserve(obj.vertices.capacity() + 3);
    for (int i = 1 ; i < 4; i++)
        obj.normals.push_back(std::stof(values[i]));
}

void parseFace(ObjectInfo& obj, const SplitArray& values, int line)
{
	if (values.size() < 4)
		throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": a face is at least 3 elements");

	obj.faces.emplace_back();
	for (auto it = values.begin() + 1; it != values.end(); it++)
	{
        obj.faces.back().reserve(obj.faces.back().capacity() + 3);

		auto elements = split(*it, '/', true);
		unsigned int index = 0;

		while (index < 3)
		{
			if (index < elements.size())
			{
				if (elements[index].length())
				{
					int converted = std::stoi(elements[index]);
					if (converted <= 0)
						throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": indices must be strictly greater than 0");
					obj.faces.back().push_back(converted);
				}
				else if (index == 0)
					throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": Vertex index cannot be omitted");
			}
			else
				obj.faces.back().push_back(0);
			index++;
		}
	}
}

SplitArray split(const std::string& str, char delim, bool acceptEmptyLines)
{
    SplitArray split;
	
	size_t start = 0;
	while (start != std::string::npos)
	{
		size_t end = str.find(delim, start);
		std::string result = str.substr(start, end - start);

		if (result.length() || acceptEmptyLines)
			split.push_back(result);

		start = (end != std::string::npos) ? end + 1 : std::string::npos;
	}

	return split;
}

void ObjectInfo::debug() const
{
    for (auto it = vertices.begin(); it != vertices.end(); it += 3)
		std::cout << "v " << *it << ' ' << *(it + 1) << ' ' << *(it + 2) << '\n';
	std::cout << '\n';

    for (auto it = textures.begin(); it != textures.end(); it += 3)
		std::cout << "vt " << *it << ' ' << *(it + 1) << ' ' << *(it + 2) << '\n';
	std::cout << '\n';

    for (auto it = normals.begin(); it != normals.end(); it += 3)
		std::cout << "vn " << *it << ' ' << *(it + 1) << ' ' << *(it + 2) << '\n';
	std::cout << '\n';
	
	std::for_each(faces.begin(), faces.end(), [](const std::vector<unsigned int>& face) {
		std::cout << "f ";
		for (size_t i = 0; i < face.size(); i++)
			std::cout << face[i] << ((i % 3 == 2) ? ' ' : '/');
		std::cout << '\n';
	});
}
