/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Material.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 19:35:40 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/03 10:39:25 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Material.hpp"
#include "parser.hpp"

Material::Material(unsigned int id)
	: id(id),
		ambientColor({ 1.f, 1.f, 1.f }), diffuseColor(ambientColor), specularColor(ambientColor),
		specularExponent(500.f), refraction(1.f), opacity(1.f) {}
