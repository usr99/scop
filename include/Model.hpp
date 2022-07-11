/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:10:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/10 23:37:55 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MODEL_HPP_
# define _MODEL_HPP_

# include <string>
# include <vector>

# include "Vector.hpp"

class Model
{
	public:

		Model(const std::string& path);

		void render() const;

	private:
		
		void _insertVertexAttribute(std::vector<float>& buffer, std::vector<float>& from, int index);

		unsigned int 			_M_VertexBuffer;
		unsigned int 			_M_IndexBuffer;
		unsigned int			_M_VerticesCount;
		// bool					_M_SmoothShadingEnabled;
};

#endif
