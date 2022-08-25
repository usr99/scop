/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:10:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/25 16:14:50 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MODEL_HPP_
# define _MODEL_HPP_

# include <string>
# include <vector>

# include "math.hpp"
# include "ColorPalette.hpp"

# define NB_COLOR_SETTINGS 4
# define NB_COLOR_SETTINGS_COMBINATIONS 6

class Model
{
	public:

		Model(const std::string& path);

		void render();
		void showSettingsPanel();

		void rotate(float angle);

		glm::mat4 getMatrix() const;

	private:

		void _insertVertexAttribute(std::vector<float>& buffer, unsigned int offset, std::vector<float>& from, unsigned int index);

		unsigned int 	_M_VertexBuffer;
		unsigned int 	_M_IndexBuffer;
		unsigned int	_M_VerticesCount;
		unsigned int	_M_IndicesCount;
		float			_M_RotationAngle;

		ColorPalette	_M_Palette;
		int				_M_RenderingMode;
		int				_M_PointSize;

		// bool			_M_HasTexture;
		// bool			_M_SmoothShadingEnabled;
};

#endif
