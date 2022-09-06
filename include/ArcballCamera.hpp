/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArcballCamera.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 14:19:47 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/05 20:31:10 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ARCBALL_CAMERA_HPP_
# define _ARCBALL_CAMERA_HPP_

# include "math.hpp"

# define INIT_POSITION 	glm::vec3(0.f, 0.f, -5.f)
# define ZOOM_MIN 		0.3f
# define ZOOM_MAX 		3.0f

class ArcballCamera
{
	public:

		ArcballCamera(float width, float height);
		
		void rotate(glm::vec2 angle);
		void translate(float x, float y);
		void zoom(float value);
		void reset();

		glm::mat4 getProjectionMatrix() const;
		glm::mat4 getViewMatrix() const;
		glm::mat4 getMatrix() const;
		glm::vec3 getPosition() const;

	private:

		/* transpose is needed to go from column-major to row-major matrices */
		inline glm::vec3 _getOrientation() { return -glm::transpose(_M_ViewMatrix)[2]; }
		inline glm::vec3 _getRightVector() { return glm::transpose(_M_ViewMatrix)[0]; }
		void _updateViewMatrix();

		glm::mat4	_M_ProjectionMatrix;
		glm::mat4	_M_ViewMatrix;
		glm::vec3	_M_Eye;
		glm::vec3	_M_LookAt;
		glm::vec3	_M_UpVector;
		glm::vec3	_M_TranslationVector;
		float		_M_Zoom;
};

#endif
