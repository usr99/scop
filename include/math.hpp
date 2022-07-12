/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 19:40:35 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/12 17:35:28 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MATH_HPP_
# define _MATH_HPP_

// # include "Matrix.hpp"

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>

// typedef Matrix<4, 4, float> mat4;
// typedef Vector<3, float> vec3;
// typedef Vector<4, float> vec4;

// template <typename T>
// T lerp(const T& u, const T& v, float t)
// {
// 	return std::fma((v - u), t, u);
// }

// template <length_t L, typename T>
// Vector<L, T> lerp(const Vector<L, T>& u, const Vector<L, T>& v, float t)
// {
// 	Vector<L, T> result;

// 	for (length_t i = 0; i < L; i++)
// 		result[i] = lerp(u[i], v[i], t);
// 	return result;
// }

// template <length_t R, length_t C, typename T>
// Matrix<R, C, T> lerp(const Matrix<R, C, T>& u, const Matrix<R, C, T>& v, float t)
// {
// 	Matrix<R, C, T> result;

// 	for (length_t i = 0; i < C; i++)
// 		result[i] = lerp(u[i], v[i], t);
// 	return result;
// }

// template <typename T>
// Matrix<4, 4, T> perspective(T fovy, T aspect, T zNear, T zFar)
// {
// 	Matrix<4, 4, T> result({{0}});
// 	T const tanHalfFovy = std::tan(fovy / static_cast<T>(2));

// 	result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
// 	result[1][1] = static_cast<T>(1) / (tanHalfFovy);
// 	result[2][2] = -(zFar + zNear) / (zFar - zNear);
// 	result[2][3] = -static_cast<T>(1);
// 	result[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
// 	return result;
// }

// template <typename T>
// T radians(T deg)
// {
//     return deg * (M_PI / 180.);
// }

#endif
