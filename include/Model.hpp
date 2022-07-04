/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:10:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/04 10:46:05 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MODEL_HPP_
# define _MODEL_HPP_

# include <string>
# include <vector>

# include "Vertex.hpp"

class Model
{
	public:

		Model(const std::string& filename);

	private:

		using Face = std::vector<unsigned int>;

		std::string 			_M_Name;
		std::vector<Vertex>		_M_Vertices;
		std::vector<Face>		_M_Indices;
};

#endif
