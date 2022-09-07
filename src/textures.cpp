/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 18:28:14 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/07 18:36:03 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "textures.hpp"
#include "BMPimage.hpp"
#include "debug.hpp"

void loadTexture(const std::string& path, unsigned int index)
{
	BMPimage img(path);
	unsigned int id;

	GLCall(glGenTextures(1, &id));
	GLCall(glActiveTexture(GL_TEXTURE0 + index));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.info.width, img.info.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.data()));
}
