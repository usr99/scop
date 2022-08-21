/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LightSource.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 12:44:19 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/21 13:02:36 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LIGHT_SOURCE_HPP_
# define _LIGHT_SOURCE_HPP_

# include "math.hpp"

class LightSource
{
	public:

		LightSource();

		glm::vec3 getColor() const;
		glm::vec3 getPosition() const;

		void showSettingsPanel();

	private:

		glm::vec3	_M_Color;
		glm::vec3	_M_InitialPosition;
		float		_M_Angle;
};

#endif
