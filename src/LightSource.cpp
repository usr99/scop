/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LightSource.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 12:51:22 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/23 17:07:44 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LightSource.hpp"
#include "imgui/imgui.h"

LightSource::LightSource()
	: _M_InitialPosition(glm::vec3(0.f, 3.f, -3.f)), _M_Angle(0.f) {}

glm::vec3
LightSource::getPosition() const
{
	return glm::rotate(_M_InitialPosition, glm::radians(_M_Angle), glm::vec3(0.f, 1.f, 0.f));
}

void
LightSource::showSettingsPanel()
{
	ImGui::Begin("Lightings");
	ImGui::SliderFloat("Angle", &_M_Angle, 0.f, 360.f);
	ImGui::End();
}
