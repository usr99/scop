/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Skybox.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 15:33:21 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/09 18:16:12 by mamartin         ###   ########.fr       */
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
		static constexpr unsigned int _M_TexturesCount = 6;
};

#endif
