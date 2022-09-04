/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Material.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 19:28:20 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/04 12:28:22 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MATERIAL_HPP_
# define _MATERIAL_HPP_

# include <map>
# include "math.hpp"

# define MAX_MATERIALS 128

struct Material
{
	Material(unsigned int id = 0);

	unsigned int id;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float specularExponent;
	float refraction;
	float opacity;
	unsigned int illuminationModel; // 0-2

	std::string texture;

	struct Uniform
	{
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;

		float shininess;
		float refraction;
		float opacity;
		unsigned int illum;
	};

	Uniform getUniformData() const;
};

#endif
