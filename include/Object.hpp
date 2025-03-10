/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Object.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 05:50:53 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 20:04:24 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _OBJECT_HPP_
# define _OBJECT_HPP_

# include <vector>
# include <string>
# include <list>
# include <map>

# include "parser.hpp"

# define MAX_MATERIALS 128
# define MAX_TEXTURES 16

struct Vertex : public std::array<unsigned int, 3>
{
	Vertex();

	unsigned int& position();
	unsigned int& uv();
	unsigned int& normal();
};

struct Polygon : public std::vector<Vertex>
{
	Polygon(const Material* materialPtr);

	const Material* mtl;
};

struct ShadingGroup
{
	ShadingGroup(bool enable);

	std::vector<Polygon>	polygons;
	bool					enabled;
};

struct Object
{
	public:

		Object(const std::string& filename);
	
		std::list<ShadingGroup>	groups;
		std::vector<ft::vec3>		vertices;
		std::vector<ft::vec3>		textures;
		std::vector<ft::vec3>		normals;
		MaterialLibrary			materials;
	
		unsigned int			vertexCount;
		ft::vec3					max;
		ft::vec3					min;

	private:
	
		using Parser = void (Object::*)(const LineElements&);

		enum Attribute
		{
			VERTEX,
			TEXTURE,
			NORMAL,
			POLYGON,
			MATERIAL_LIB,
			MATERIAL
		};

		void _parseVertex(const LineElements&);
		void _parseUV(const LineElements&);
		void _parseNormal(const LineElements&);
		void _parsePolygon(const LineElements&);
		void _parseMaterialLibrary(const LineElements&);
		void _parseMaterial(const LineElements&);

		const Material* _M_currentMaterial;

		static constexpr Parser _ParserFunctions[] = {
			&Object::_parseVertex,
			&Object::_parseUV,
			&Object::_parseNormal,
			&Object::_parsePolygon,
			&Object::_parseMaterialLibrary,
			&Object::_parseMaterial
		};
};

#endif
