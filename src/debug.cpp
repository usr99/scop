/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 04:04:57 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/03 10:15:06 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "debug.hpp"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cerr << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << "\n";
		return false;
	}
	return true;
}

void debug(const Object& obj)
{
    for (auto it = obj.vertices.begin(); it != obj.vertices.end(); it++)
		std::cout << "v " << it->x << ' ' << it->y << ' ' << it->z << '\n';
	std::cout << '\n';

    for (auto it = obj.textures.begin(); it != obj.textures.end(); it++)
		std::cout << "vt " << it->x << ' ' << it->y << ' ' << it->z << '\n';
	std::cout << '\n';

    for (auto it = obj.normals.begin(); it != obj.normals.end(); it++)
		std::cout << "vn " << it->x << ' ' << it->y << ' ' << it->z << '\n';
	std::cout << '\n';

	for (auto group = obj.groups.begin(); group != obj.groups.end(); group++)
	{
		for (auto polygon = group->polygons.begin(); polygon != group->polygons.end(); polygon++)
		{
			std::cout << "f ";
			for (auto vertex = polygon->begin(); vertex != polygon->end(); vertex++)
				std::cout << vertex->at(0) << '/' << vertex->at(1) << '/' << vertex->at(2) << ' ';
			std::cout << '\n';
		}
		std::cout << '\n';
	}
	std::cout << "max: " << obj.max << "\nmin: " << obj.min << '\n';

	for (auto mtl = obj.materials.begin(); mtl != obj.materials.end(); mtl++)
	{
		std::cout << mtl->first << ":\n\n";
		debug(mtl->second);
	}
}

void debug(const Material& mtl)
{
	std::cout << "Ka " << mtl.ambientColor << '\n';
	std::cout << "Kd " << mtl.diffuseColor << '\n';
	std::cout << "Ks " << mtl.specularColor << '\n';

	std::cout << "specular exponent " << mtl.specularExponent << '\n';
	std::cout << "index of refraction " << mtl.refraction << '\n';
	std::cout << "opacity " << mtl.opacity << '\n';
	std::cout << "illumation model " << mtl.illuminationModel << '\n';
	
	std::cout << "texture " << mtl.texture << '\n';
}
