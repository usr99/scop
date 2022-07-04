/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:45:16 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/04 15:26:32 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <stdexcept>

#include "Model.hpp"
#include "utils.hpp"

Model::Model(const std::string& path)
{
	/* Check file extension */
	size_t extensionIndex = path.find(".obj", path.length() - 4);
	if (extensionIndex == std::string::npos)
		throw std::invalid_argument("Bad file extension, must be .obj");

	/* Open object file */
	std::ifstream file(path);
	if (!file)
		throw std::invalid_argument(path + " not found");

	std::vector<Model> models;
	std::string line;
	int lineIndex = 1;
	
	/* Parsing routine */
	while (file.good())
	{
		std::getline(file, line);
		auto components = split(line);

		if (components.size())
		{
			if (components[0] == "v")
 				_parseVertex(components, lineIndex);
			else if (components[0] == "vt")
 				_parseTextureCoordinates(components, lineIndex);
			else if (components[0] == "vn")
 				_parseNormalVector(components, lineIndex);
			else if (components[0] == "f")
 				_parseFace(components, lineIndex);
			else if (components[0] == "s")
				_M_SmoothShadingEnabled = (components[1] == "on");
		}

		lineIndex++;
	}

	/* Left the loop before the file was fully read */
	if (file.bad())
		throw std::runtime_error("Fatal error occured while reading your file");
}

void Model::_parseVertex(const std::vector<std::string>& values, int line)
{
	if (values.size() < 4 || values.size() > 5)
		throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": invalid number of coordinates");

	_M_Vertices.push_back(VectorType({
		std::stof(values[1]),
		std::stof(values[2]),
		std::stof(values[3])
	}));

	/* Divide all coordinates by w if it is specified */
	if (values.size() == 5)
		_M_Vertices.back() /= std::stof(values[4]);
}

void Model::_parseTextureCoordinates(const std::vector<std::string>& values, int line)
{
	if (values.size() < 2 || values.size() > 4)
		throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": invalid number of coordinates");

	_M_TexCoordinates.push_back(VectorType());
	for (unsigned int i = 1; i < values.size(); i++)
		_M_TexCoordinates.back()[i - 1] = std::stof(values[i]);
}

void Model::_parseNormalVector(const std::vector<std::string>& values, int line)
{
	if (values.size() != 4)
		throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": invalid number of coordinates");

	_M_Normals.push_back(VectorType({
		std::stof(values[1]),
		std::stof(values[2]),
		std::stof(values[3])
	}));
}

void Model::_parseFace(const std::vector<std::string>& values, int line)
{
	if (values.size() < 4)
		throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": a face is at least 3 elements");

	_M_Indices.push_back(Face());
	for (auto it = values.begin() + 1; it != values.end(); it++)
	{
		auto elements = split(*it, '/', true);

		_M_Indices.back().push_back(std::array<unsigned int, 3>());
		unsigned int index = 0;
		while (index < elements.size())
		{
			if (elements[index].length())
			{
				int converted = std::stoi(elements[index]);
				if (converted <= 0)
					throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": indices must be strictly greater than 0");
				_M_Indices.back().back()[index] = converted;
			}
			else if (index == 0)
				throw std::invalid_argument("Bad syntax at line " + std::to_string(line) + ": Vertex index cannot be omitted");
			index++;
		}
	}
}

#include <iostream>

void Model::debug() const
{
	std::for_each(_M_Vertices.begin(), _M_Vertices.end(), [](VectorType v) {
		std::cout << "v " << v << '\n';
	});

	std::cout << '\n';

	std::for_each(_M_TexCoordinates.begin(), _M_TexCoordinates.end(), [](VectorType v) {
		std::cout << "vt " << v << '\n';
	});

	std::cout << '\n';

	std::for_each(_M_Normals.begin(), _M_Normals.end(), [](VectorType v) {
		std::cout << "vn " << v << '\n';
	});

	std::cout << '\n';

	std::for_each(_M_Indices.begin(), _M_Indices.end(), [](Face f) {
		std::cout << "f ";
		std::for_each(f.begin(), f.end(), [](std::array<unsigned int, 3> arr) {
			std::cout << arr[0] << "/" << arr[1] << "/" << arr[2] << " ";
		});
		std::cout << '\n';
	});

}
