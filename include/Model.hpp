/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:10:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/03 12:41:38 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MODEL_HPP_
# define _MODEL_HPP_

# include <string>
# include <vector>

# include "math.hpp"
# include "ColorPalette.hpp"
# include "Object.hpp"
# include "ShaderProgram.hpp"

class Model
{
	public:

		Model(const std::string& path, ShaderProgram& shader);

		void render();
		void showSettingsPanel();

		void rotate(float angle);

		glm::mat4 getMatrix() const;

	private:

		void _insertVertexAttribute(std::vector<float>& buffer, unsigned int offset, std::vector<glm::vec3>& from, unsigned int index);

		Object			_M_ObjectInfo;

		unsigned int 	_M_VertexBuffer;
		unsigned int 	_M_IndexBuffer;
		unsigned int	_M_UniformBuffer;
		unsigned int	_M_IndicesCount;

		ColorPalette	_M_Palette;
		float			_M_RotationAngle;
		int				_M_RenderingMode;
		int				_M_PointSize;
};

#endif
