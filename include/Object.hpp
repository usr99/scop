/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 05:50:53 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/30 15:13:07 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _OBJECT_HPP_
# define _OBJECT_HPP_

# include <vector>
# include <list>
# include <string>

# include "math.hpp"

struct Vertex : public std::array<unsigned int, 3>
{
	Vertex();

	unsigned int& position();
	unsigned int& uv();
	unsigned int& normal();
};

struct ShadingGroup
{
	using Polygon = std::vector<Vertex>;

	ShadingGroup(bool smoothShadingOn);

	std::vector<Polygon> polygons;
	bool				 enabled;
};

struct Object
{
	public:

		Object(const std::string& filename);
	
		void debug() const;
	
		std::list<ShadingGroup>		groups;
		std::vector<glm::vec3>		vertices;
		std::vector<glm::vec3>		textures;
		std::vector<glm::vec3>		normals;
	
		unsigned int				vertexCount;
		vec3						max;
		vec3						min;

	private:
	
		using indices_t = std::pair<size_t, size_t>;
		using Parser = void (Object::*)(const std::string&, const std::vector<indices_t>&);

		enum Attribute
		{
			VERTEX,
			TEXTURE,
			NORMAL,
			POLYGON
		};

		void _parseVertex(const std::string& line, const std::vector<indices_t>& indices);
		void _parseUV(const std::string& line, const std::vector<indices_t>& indices);
		void _parseNormal(const std::string& line, const std::vector<indices_t>& indices);
		void _parsePolygon(const std::string& line, const std::vector<indices_t>& indices);
		std::vector<indices_t> _split(const std::string& str, char delim = ' ', bool allowNullStrings = false);

		static constexpr Parser _ParserFunctions[4] = {
			&Object::_parseVertex,
			&Object::_parseUV,
			&Object::_parseNormal,
			&Object::_parsePolygon			
		};
};

# if 0
typedef std::vector<std::string> SplitArray;

ObjectInfo loadObjectFile(std::ifstream& file);

void parseVertex(ObjectInfo& obj, const SplitArray& values, int line);
void parseTextureCoordinates(ObjectInfo& obj, const SplitArray& values, int line);
void parseNormalVector(ObjectInfo& obj, const SplitArray& values, int line);
void parseFace(ObjectInfo& obj, const SplitArray& values, int line);

SplitArray split(const std::string& str, char delim = ' ', bool acceptEmptyLines = false);
# endif

#endif
