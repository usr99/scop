/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArcballCamera.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 14:28:10 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 21:09:32 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ArcballCamera.hpp"

ArcballCamera::ArcballCamera(float width, float height)
	: 	_M_ProjectionMatrix(ft::perspective(ft::radians(45.0f), width / height, 0.1f, 100.0f)),
		_M_Eye(INIT_POSITION), _M_LookAt(ft::vec3()), _M_UpVector(ft::vec3({ 0.f, 1.f, 0.f })),
		_M_Zoom(1.f)
{
	_updateViewMatrix();
}

void
ArcballCamera::rotate(ft::vec2 angle)
{
	auto sgn = [](float x) {
		return (x > 0) - (x < 0); // returns 1 if x is positive, -1 if it's not
	};

	/*
	** Stop the camera when the orientation and up vectors are colinears (cosinus of 1 or -1)
	** if the camera is above or below the object, keep it from going further up or down
	*/
	float cosAngle = _getOrientation().dot(_M_UpVector);
	if (cosAngle * sgn(angle.y()) < -0.99f)
		angle.y() = 0.f;

	ft::vec4 position(_M_Eye, 1.f);
	ft::mat4 rotationX;
	rotationX = ft::rotate(rotationX, angle.x(), _M_UpVector);
	position = rotationX * position;

	ft::mat4 rotationY;
	rotationY = ft::rotate(rotationY, angle.y(), _getRightVector());
	position = rotationY * position;

	_M_Eye = position;
	_updateViewMatrix();
}

void
ArcballCamera::translate(float x, float y)
{
	/*
	** Coordinates are divided by the zoom factor
	** because of the perspective, the further you are from the object
	** the more the camera needs to move to travel the same distance on the screen
	*/
	const ft::vec2 save = _M_TranslationVector;
	_M_TranslationVector.x() -= (x / _M_Zoom);
	_M_TranslationVector.y() += (y / _M_Zoom);
	_updateViewMatrix();

	/* Compute the projection of the origin on the window */
	const ft::vec4 origin({ 0.f, 0.f, 0.f, 1.f });
	const ft::vec4 projected = getMatrix() * origin;

	/* Check that the origin is not out of the window borders */
	for (int i = 0; i < 2; i++)
	{
		if (projected[i] < -projected.w() || projected[i] > projected.w())
			_M_TranslationVector[i] = save[i]; // restore previous coordinates
	}
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
	_M_Zoom = std::max(ZOOM_MIN, std::min(ZOOM_MAX, _M_Zoom));
	_updateViewMatrix();
}

void
ArcballCamera::reset()
{
	_M_Eye = INIT_POSITION;
	_M_TranslationVector = ft::vec3();
}

ft::mat4
ArcballCamera::getProjectionMatrix() const
{
	return _M_ProjectionMatrix;
}

ft::mat4
ArcballCamera::getViewMatrix() const
{
	return _M_ViewMatrix;
}

ft::mat4
ArcballCamera::getMatrix() const
{
	return _M_ProjectionMatrix * _M_ViewMatrix;
}

ft::vec3
ArcballCamera::getPosition() const
{
	return _M_Eye;
}

void
ArcballCamera::_updateViewMatrix()
{
	_M_ViewMatrix = ft::lookAt<float>(_M_Eye / _M_Zoom, _M_LookAt, _M_UpVector);
	_M_ViewMatrix = ft::translate(_M_ViewMatrix, _M_TranslationVector);
}
