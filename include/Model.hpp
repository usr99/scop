/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:10:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/12 11:52:45 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MODEL_HPP_
# define _MODEL_HPP_

# include <string>
# include <vector>

# include "math.hpp"

class Model
{
	public:

		Model(const std::string& path);

		void render() const;

		void translate(const glm::vec3& direction);
		void rotate(float angle, const glm::vec3& axis);
		void scale(float factor);

		inline const glm::mat4& getMatrix() const { return _M_ModelMatrix; };

	private:
		
		void _insertVertexAttribute(std::vector<float>& buffer, std::vector<float>& from, int index);

		unsigned int 			_M_VertexBuffer;
		unsigned int 			_M_IndexBuffer;
		unsigned int			_M_VerticesCount;
		glm::mat4				_M_ModelMatrix;

		// bool					_M_SmoothShadingEnabled;
};

#endif
