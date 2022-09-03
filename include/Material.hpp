/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Material.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 19:28:20 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/03 10:43:54 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MATERIAL_HPP_
# define _MATERIAL_HPP_

# include <map>
# include "math.hpp"

struct Material
{
	Material(unsigned int id);

	unsigned int id;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float specularExponent;
	float refraction;
	float opacity;
	unsigned int illuminationModel; // 0-2

	std::string texture;
};

#endif
