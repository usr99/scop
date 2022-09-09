/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Material.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 19:35:40 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 20:20:59 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Material.hpp"
#include "parser.hpp"
#include "debug.hpp"

Material::Material(unsigned int id)
	: id(id),
		ambientColor({ 1.f, 1.f, 1.f }), diffuseColor(ambientColor), specularColor(ambientColor),
		specularExponent(500.f), refraction(1.f), opacity(1.f), illuminationModel(1),
		texture(Material::DefaultTexture) {}

Material::Uniform
Material::getUniformData() const
{
	Uniform result;

	result.ambient = ft::vec4(ambientColor, 1.f);
	result.diffuse = ft::vec4(diffuseColor, 1.f);
	result.specular = ft::vec4(specularColor, 1.f);

	result.shininess = specularExponent;
	result.refraction = refraction;
	result.opacity = opacity;
	result.illum = illuminationModel;

	return result;
}
