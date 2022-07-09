/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:45:16 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/09 07:23:07 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <stdexcept>

#include "Model.hpp"
#include "parser.hpp"

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

	ObjectInfo obj = loadObjectFile(file);
	file.close();
	
	/* Create buffers for OpenGL rendering */

}
