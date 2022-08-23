/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:10:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/22 19:43:15 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MODEL_HPP_
# define _MODEL_HPP_

# include <string>
# include <vector>

# include "math.hpp"

# define NB_COLOR_SETTINGS 4
# define NB_COLOR_SETTINGS_COMBINATIONS 6

class Model
{
	public:

		Model(const std::string& path);

		void render();
		void showSettingsPanel();

		void translate(const glm::vec3& direction);
		void rotate(float angle, const glm::vec3& axis);
		void scale(float factor);

		inline const glm::mat4& getMatrix() const { return _M_ModelMatrix; };

	private:

		enum ColorModeSettings
		{
			SHADES_MODE,
			RANDOM_MODE,
			COLOR_MODE,
			GRADIENT_MODE
		};

		void _insertVertexAttribute(std::vector<float>& buffer,int offset, std::vector<float>& from, int index);
		char _getCurrentColorModeMask() const;
		void _generateColorPalette();

		unsigned int 	_M_VertexBuffer;
		unsigned int 	_M_IndexBuffer;
		unsigned int	_M_VerticesCount;
		unsigned int	_M_IndicesCount;
		glm::mat4		_M_ModelMatrix;

		bool			_M_ColorMode[NB_COLOR_SETTINGS];
		char			_M_ColorModeMask;

		// bool			_M_HasTexture;
		// bool			_M_SmoothShadingEnabled;
};

#endif
