/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Material.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 19:28:20 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 20:09:01 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MATERIAL_HPP_
# define _MATERIAL_HPP_

# include <map>
# include "math.hpp"

struct Material
{
	Material(unsigned int id = 0);

	struct Uniform
	{
		ft::vec4 ambient;
		ft::vec4 diffuse;
		ft::vec4 specular;

		float shininess;
		float refraction;
		float opacity;
		unsigned int illum;
	};

	Uniform getUniformData() const;

	unsigned int id;
	ft::vec3 ambientColor;
	ft::vec3 diffuseColor;
	ft::vec3 specularColor;
	float specularExponent;
	float refraction;
	float opacity;
	unsigned int illuminationModel; // 0-2
	std::string texture;
	
	static constexpr const char* DefaultTexture = "resources/textures/block.bmp";
};

#endif
