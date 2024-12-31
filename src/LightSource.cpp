/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LightSource.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 12:51:22 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 20:19:20 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LightSource.hpp"
#include "imgui.h"

LightSource::LightSource()
	: _M_InitialPosition(ft::vec3({ 0.f, 3.f, -3.f })), _M_Angle(0.f) {}

ft::vec3
LightSource::getPosition() const
{
	return ft::rotate(_M_InitialPosition, ft::radians(_M_Angle), ft::vec3({ 0.f, 1.f, 0.f }));
}

void
LightSource::showSettingsPanel()
{
	ImGui::SliderFloat("light angle", &_M_Angle, 0.f, 360.f);
}
