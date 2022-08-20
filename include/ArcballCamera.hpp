/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ArcballCamera.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 14:19:47 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/20 15:34:03 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ARCBALL_CAMERA_HPP_
# define _ARCBALL_CAMERA_HPP_

# include "math.hpp"

class ArcballCamera
{
	public:

		ArcballCamera(const glm::vec3& eye);
		
		inline glm::mat4 getMatrix() { return _M_ViewMatrix; };
		// inline glm::vec3 getPosition() { return _M_Eye; };

		void rotate(glm::vec2 angle);

	private:

		/* transpose is needed to go from column-major to row-major matrices */
		inline glm::vec3 _getOrientation() { return -glm::transpose(_M_ViewMatrix)[2]; }
		inline glm::vec3 _getRightVector() { return glm::transpose(_M_ViewMatrix)[0]; }
		void _updateViewMatrix();

		glm::mat4 _M_ViewMatrix;
		glm::vec3 _M_Eye;
		glm::vec3 _M_LookAt;
		glm::vec3 _M_UpVector;
};

#endif
