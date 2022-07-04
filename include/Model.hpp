/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:10:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/04 14:55:47 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MODEL_HPP_
# define _MODEL_HPP_

# include <string>
# include <vector>

# include "Vector.hpp"

class Model
{
	public:

		Model(const std::string& path);

		void debug() const;

	private:

		using VectorType = Vector<3, float>;
		using Face = std::vector<std::array<unsigned int, 3>>;
		
		/* Specific parsing functions */
		void _parseVertex(const std::vector<std::string>& values, int line);
		void _parseTextureCoordinates(const std::vector<std::string>& values, int line);
		void _parseNormalVector(const std::vector<std::string>& values, int line);
		void _parseFace(const std::vector<std::string>& values, int line);

		/* Utils */
		std::vector<std::string> _split(const std::string& str, char delim = ' ');

		std::string 			_M_Name;
		std::vector<VectorType>	_M_Vertices;
		std::vector<VectorType>	_M_TexCoordinates;
		std::vector<VectorType>	_M_Normals;
		std::vector<Face>		_M_Indices;
		bool					_M_SmoothShadingEnabled;
};

#endif
