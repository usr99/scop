/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LightSource.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 12:44:19 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 20:04:24 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LIGHT_SOURCE_HPP_
# define _LIGHT_SOURCE_HPP_

# include "math.hpp"

class LightSource
{
	public:

		LightSource();

		ft::vec3 getPosition() const;

		void showSettingsPanel();

	private:

		const ft::vec3	_M_InitialPosition;
		float			_M_Angle;
};

#endif
