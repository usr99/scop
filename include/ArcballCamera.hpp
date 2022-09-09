/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArcballCamera.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 14:19:47 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 20:46:35 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ARCBALL_CAMERA_HPP_
# define _ARCBALL_CAMERA_HPP_

# include "math.hpp"

# define INIT_POSITION 	ft::vec3({ 0.f, 0.f, -5.f })
# define ZOOM_MIN 		0.3f
# define ZOOM_MAX 		3.0f

class ArcballCamera
{
	public:

		ArcballCamera(float width, float height);
		
		void rotate(ft::vec2 angle);
		void translate(float x, float y);
		void zoom(float value);
		void reset();

		ft::mat4 getProjectionMatrix() const;
		ft::mat4 getViewMatrix() const;
		ft::mat4 getMatrix() const;
		ft::vec3 getPosition() const;

	private:

		inline ft::vec3 _getOrientation() { return -(_M_ViewMatrix[2]); }
		inline ft::vec3 _getRightVector() { return _M_ViewMatrix[0]; }
		void _updateViewMatrix();

		ft::mat4	_M_ProjectionMatrix;
		ft::mat4	_M_ViewMatrix;
		ft::vec3	_M_Eye;
		ft::vec3	_M_LookAt;
		ft::vec3	_M_UpVector;
		ft::vec3	_M_TranslationVector;
		float		_M_Zoom;
};

#endif
