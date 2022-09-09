/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:10:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 20:05:11 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MODEL_HPP_
# define _MODEL_HPP_

# include <string>
# include <vector>

# include "math.hpp"
# include "ColorPalette.hpp"
# include "ShaderProgram.hpp"

class Model
{
	public:

		Model(const std::string& path);

		void render(int primitives);
		void showSettingsPanel();

		void rotate(float angle);

		ft::mat4 getMatrix() const;

	private:

		void _insertVertexAttribute(std::vector<float>& buffer, unsigned int offset, std::vector<ft::vec3>& from, unsigned int index);

		unsigned int 	_M_VAO;
		unsigned int 	_M_VertexBuffer;
		unsigned int 	_M_IndexBuffer;
		unsigned int	_M_UniformBuffer;
		unsigned int	_M_IndicesCount;

		ColorPalette	_M_Palette;
		float			_M_RotationAngle;
};

#endif
