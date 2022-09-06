/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Skybox.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 15:33:21 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/05 20:45:25 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SKYBOX_HPP_
# define _SKYBOX_HPP_

# include "ShaderProgram.hpp"
# include "BMPimage.hpp"

class Skybox
{
	public:

		Skybox();
		void render() const;

	private:

		unsigned int _M_VAO;
		unsigned int _M_VertexBuffer;
		unsigned int _M_IndexBuffer;
		unsigned int _M_TextureId;
		
		static constexpr unsigned int _M_IndicesCount = 36;
		static constexpr const char* _M_TexturesFiles[] = {
			"resources/textures/right.bmp",
			"resources/textures/left.bmp",
			"resources/textures/top.bmp",
			"resources/textures/bottom.bmp",
			"resources/textures/front.bmp",
			"resources/textures/back.bmp"
		};
		static constexpr unsigned int _M_TexturesCount = 6;
};

#endif
