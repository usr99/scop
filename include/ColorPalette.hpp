/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ColorPalette.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 14:58:06 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/23 15:29:23 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _COLOR_PALETTE_HPP_
# define _COLOR_PALETTE_HPP_

# include <vector>

# define DEFAULT_GREY 0.5f

class ColorPalette
{
	public:

		ColorPalette();

		void reserve(std::size_t capacity);
		void push(float grey);
		void update();
		void showSettings();

	private:

		std::vector<float>	_M_Array;

		bool				_M_IsEnabled;
		bool				_M_LastState;
};

#endif
