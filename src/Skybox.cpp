/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Skybox.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 18:02:07 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/05 20:46:23 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Skybox.hpp"
#include "BMPimage.hpp"
#include "debug.hpp"

static const float _M_VerticesData[] = {
	-25.f,  25.f,  25.f, 
	 25.f,  25.f,  25.f, 
	 25.f, -25.f,  25.f, 
	-25.f, -25.f,  25.f, 

	-25.f,  25.f, -25.f, 
	 25.f,  25.f, -25.f, 
	 25.f, -25.f, -25.f, 
	-25.f, -25.f, -50.f
};

static const unsigned int _M_IndicesData[] = {
	0, 1, 2,	0, 2, 3,
	4, 5, 6,	4, 6, 7,
	0, 4, 5,	0, 5, 1,
	1, 5, 6,	1, 6, 2,
	0, 4, 7,	0, 7, 3,
	3, 7, 6,	3, 6, 2
};

Skybox::Skybox()
{
	GLCall(glGenVertexArrays(1, &_M_VAO));
	GLCall(glBindVertexArray(_M_VAO));

	GLCall(glGenBuffers(1, &_M_VertexBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _M_VertexBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &_M_VerticesData, GL_STATIC_DRAW));

	GLCall(glGenBuffers(1, &_M_IndexBuffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _M_IndexBuffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), &_M_IndicesData, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const char*)0));

	GLCall(glGenTextures(1, &_M_TextureId));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _M_TextureId));

	for (unsigned int i = 0; i < _M_TexturesCount; i++)
	{
		BMPimage img(_M_TexturesFiles[i]);
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, img.info.width, img.info.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.data()));
	}

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));	
}

void
Skybox::render() const
{
	GLCall(glBindVertexArray(_M_VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _M_VertexBuffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _M_IndexBuffer));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, _M_TextureId));
	GLCall(glDepthMask(GL_FALSE));

	GLCall(glDrawElements(GL_TRIANGLES, _M_IndicesCount, GL_UNSIGNED_INT, 0));
	GLCall(glDepthMask(GL_TRUE));
}
