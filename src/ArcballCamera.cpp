/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArcballCamera.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 14:28:10 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/20 17:34:37 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ArcballCamera.hpp"

ArcballCamera::ArcballCamera(const glm::vec3& eye)
	: _M_Eye(eye), _M_LookAt(glm::vec3()), _M_UpVector(glm::vec3(0.f, 1.f, 0.f)),
		_M_Zoom(1.f)
{
	_updateViewMatrix();
}

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
ArcballCamera::zoom(float value)
{
	/*
	** Zoom in or out based on the scrolling of the mouse wheel
	** divide the value by 0.03 to avoid losing control of the zoom
	** keep the zoom factor between 0.3 and 1.5 to keep the camera from going into the object
	** or to keep the object from clipping out of the view frustrum
	*/
	_M_Zoom += value * 0.03f;
	_M_Zoom = std::max(0.3f, std::min(1.5f, _M_Zoom));
	_updateViewMatrix();
}

void
ArcballCamera::_updateViewMatrix()
{
	_M_ViewMatrix = glm::lookAt(_M_Eye / _M_Zoom, _M_LookAt, _M_UpVector);
}
