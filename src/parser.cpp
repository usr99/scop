/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 19:52:33 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/03 23:36:49 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>

#include "parser.hpp"
#include "Object.hpp"

void checkFileExtension(const std::string& filename, const std::string& extension)
{
	size_t extensionIndex = filename.find(extension, filename.length() - extension.length());
	if (extensionIndex == std::string::npos)
		throw std::invalid_argument("Bad file extension, must be " + extension);
}

LineElements split(const std::string& line, char delimiter, bool allowEmptyLines)
{
	LineElements split;

	size_t start = 0;
	while (start < line.length())
	{
		size_t end = line.find(delimiter, start);
		std::string result = line.substr(start, end - start);

		if (result.length() || allowEmptyLines)
			split.push_back(result);

		start = (end != std::string::npos) ? end + 1 : std::string::npos;
	}
	return split;	
}

void loadMaterialLibrary(const std::string& filename, MaterialLibrary& lib)
{
	try
	{
		checkFileExtension(filename, ".mtl");
		std::ifstream file(filename);
		if (!file)
			throw std::runtime_error(strerror(errno));
		
		std::string line;
		MaterialLibrary newlib;
		MaterialLibrary::iterator last = newlib.end();
		while (std::getline(file, line))
		{
			auto elements = split(line);
			if (elements.size())
			{
				auto statement = elements.front();
				if (statement == "newmtl")
				{
					if (elements.size() != 2)
						throw std::invalid_argument("\"newmtl\" can define only one material at a time");
					
					auto result = newlib.insert(std::make_pair(elements[1], Material(lib.size() + newlib.size())));
					if (result.second == false)
						throw std::invalid_argument("material " + elements[1] + " is defined twice");
					last = result.first;
				}
				else if (statement.front() != '#')
				{
					if (last == newlib.end())
						throw std::invalid_argument("\"newmtl\" statement must precede any other statements");

					switch (statement.front())
					{
						case 'K':
							switch (statement[1])
							{
								case 'a':
									last->second.ambientColor = parseFloat3(elements);
									break;
								case 'd':
									last->second.diffuseColor = parseFloat3(elements);
									break;
								case 's':
									last->second.specularColor = parseFloat3(elements);
									break;
							}
							break;
						case 'N':
							switch (statement[1])
							{
								case 'i':
									last->second.refraction = parseFloat1(elements);
									break;
								case 's':
									last->second.specularExponent = parseFloat1(elements);
									break;
							}
							break;
						case 'd':
							last->second.opacity = parseFloat1(elements);
							break;
						case 'i':
							if (statement == "illum")
							{
								last->second.illuminationModel = parseUInt1(elements);
								if (last->second.illuminationModel > 2)
									throw std::invalid_argument("incorrect illumination model");
							}
							break;
						case 'm':
							if (statement == "map_Kd")
								last->second.texture = parseString1(elements);
							break;
					}
				}
			}
		}
		lib.insert(newlib.begin(), newlib.end());
	}
	catch (const std::exception& e)
	{
		std::cerr << filename + ": " + e.what() << '\n';
	}
}

std::string parseString1(const LineElements& elements)
{
	if (elements.size() != 2)
		throw std::invalid_argument("incorrect number of components for " + elements[0] + " statement");
	return elements[1];
}

unsigned int parseUInt1(const LineElements& elements)
{
	if (elements.size() != 2)
		throw std::invalid_argument("incorrect number of components for " + elements[0] + " statement");
	return std::stoi(elements[1]);
}

float parseFloat1(const LineElements& elements)
{
	if (elements.size() != 2)
		throw std::invalid_argument("incorrect number of components for " + elements[0] + " statement");
	return std::stof(elements[1]);
}

vec3 parseFloat3(const LineElements& elements)
{
	vec3 result;

	if (elements.size() != 4)
		throw std::invalid_argument("incorrect number of components for " + elements[0] + " statement");
	result.x() = std::stof(elements[1]);
	result.y() = std::stof(elements[2]);
	result.z() = std::stof(elements[3]);
	return result;
}
