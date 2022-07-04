/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vertex.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:27:21 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/04 10:46:23 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _VERTEX_HPP_
# define _VERTEX_HPP_

# include "Vector.hpp"

struct Vertex
{
	using VectorType = Vector<3, float>;

	VectorType vertex;
	VectorType texCoordinates;
	VectorType normal;
};

#endif
