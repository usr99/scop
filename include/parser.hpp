/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 05:50:53 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/09 06:39:39 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PARSER_HPP_
# define _PARSER_HPP_

# include <fstream>
# include <vector>
# include <string>

struct ObjectInfo
{
	std::vector<std::vector<unsigned int>>	indices;
	std::vector<float>			            vertices;
	std::vector<float>			            textures;
	std::vector<float>			            normals;

	void debug() const;
};

typedef std::vector<std::string> SplitArray;

ObjectInfo loadObjectFile(std::ifstream& file);

void parseVertex(ObjectInfo& obj, const SplitArray& values, int line);
void parseTextureCoordinates(ObjectInfo& obj, const SplitArray& values, int line);
void parseNormalVector(ObjectInfo& obj, const SplitArray& values, int line);
void parseFace(ObjectInfo& obj, const SplitArray& values, int line);

SplitArray split(const std::string& str, char delim = ' ', bool acceptEmptyLines = false);

#endif
