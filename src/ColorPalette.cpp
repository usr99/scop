/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ColorPalette.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 15:04:11 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/23 15:45:55 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ColorPalette.hpp"

#include "GL/glew.h"
#include "imgui/imgui.h"
#include "debug.hpp"

ColorPalette::ColorPalette()
	: _M_IsEnabled(false), _M_LastState(true) {}

void
ColorPalette::reserve(size_t capacity)
{
	_M_Array.reserve(capacity);
}

void
ColorPalette::push(float grey)
{
	_M_Array.insert(_M_Array.end(), 3, grey);
}

void
ColorPalette::showSettings()
{
	ImGui::Checkbox("Toggle shades of grey", &_M_IsEnabled);
}

void
ColorPalette::update()
{
	if (_M_IsEnabled != _M_LastState)
	{
		size_t count = _M_Array.capacity();

		/*
		** Copy the color data to GPU memory for later rendering
		** only the last quarter of the buffer is written
		*/
		if (!_M_IsEnabled)
		{
			GLCall(glBufferSubData(
				GL_ARRAY_BUFFER,
				sizeof(float) * count * 3,
				sizeof(float) * count,
				std::vector<float>(count, DEFAULT_GREY).data() // medium grey only
			));
		}
		else
		{
			GLCall(glBufferSubData(
				GL_ARRAY_BUFFER,
				sizeof(float) * count * 3,
				sizeof(float) * count,
				_M_Array.data() // shades of grey
			));			
		}
		_M_LastState = _M_IsEnabled;
	}
}
