/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArcballCamera.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 14:28:10 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/20 16:54:59 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ArcballCamera.hpp"

ArcballCamera::ArcballCamera(const glm::vec3& eye)
	: _M_Eye(eye), _M_LookAt(glm::vec3()), _M_UpVector(glm::vec3(0.f, 1.f, 0.f))
{
	_updateViewMatrix();
}
#include <iostream>
void
ArcballCamera::rotate(glm::vec2 angle)
{
	auto sgn = [](float x) {
		return (x > 0) - (x < 0); // returns 1 if x is positive, -1 if it's not
	};

	/*
	** Stop the camera when the orientation and up vectors are colinears (cosinus of 1 or -1)
	** if the camera is above or below the object, keep it from going further up or down
	*/
	float cosAngle = glm::dot(_getOrientation(), _M_UpVector);
	if (cosAngle * sgn(angle.y) < -0.99f)
		angle.y = 0.f;

	glm::vec4 position(_M_Eye.x, _M_Eye.y, _M_Eye.z, 1);
	glm::mat4 rotationX(1.f);
	rotationX = glm::rotate(rotationX, -angle.x, _M_UpVector);
	position = rotationX * position;

	glm::mat4 rotationY(1.f);
	rotationY = glm::rotate(rotationY, -angle.y, _getRightVector());
	position = rotationY * position;

	_M_Eye = position;
	_updateViewMatrix();
}

void
ArcballCamera::_updateViewMatrix()
{
	_M_ViewMatrix = glm::lookAt(_M_Eye, _M_LookAt, _M_UpVector);
}
