/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 19:40:35 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 21:09:22 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MATH_HPP_
# define _MATH_HPP_

# include <iostream>

# include "Matrix.hpp"
# include "Vector.hpp"

namespace ft
{
	using mat4 = Matrix<4, 4, float>;
	using vec2 = Vector<2, float>;
	using vec3 = Vector<3, float>;
	using vec4 = Vector<4, float>;

	template <typename T>
	T lerp(const T& u, const T& v, float t)
	{
		return std::fma((v - u), t, u);
	}

	template <length_t L, typename T>
	Vector<L, T> lerp(const Vector<L, T>& u, const Vector<L, T>& v, float t)
	{
		Vector<L, T> result;

		for (length_t i = 0; i < L; i++)
			result[i] = lerp(u[i], v[i], t);
		return result;
	}

	template <length_t R, length_t C, typename T>
	Matrix<R, C, T> lerp(const Matrix<R, C, T>& u, const Matrix<R, C, T>& v, float t)
	{
		Matrix<R, C, T> result;

		for (length_t i = 0; i < C; i++)
			result[i] = lerp(u[i], v[i], t);
		return result;
	}

	template <typename T>
	Matrix<4, 4, T> perspective(T fovy, T aspect, T zNear, T zFar)
	{
		Matrix<4, 4, T> result({{0}});
		T const tanHalfFovy = std::tan(fovy / static_cast<T>(2));

		result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
		result[1][1] = static_cast<T>(1) / (tanHalfFovy);
		result[2][2] = -(zFar + zNear) / (zFar - zNear);
		result[3][2] = -static_cast<T>(1);
		result[2][3] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
		result[3][3] = T(0);
		return result;
	}

	template <typename T>
	T radians(T deg)
	{
	    return deg * (M_PI / 180.);
	}

	template <length_t L, typename T>
	Vector<L, T> normalize(Vector<L, T> v)
	{
		const float norm = v.norm();
		if (norm)
			v /= norm;
		return v;
	}

	template <typename T>
	Matrix<4, 4, T> translate(Matrix<4, 4, T> mat, const Vector<3, T>& direction)
	{
		for (int i = 0; i < 3; i++)
			mat[i][3] += direction[i];
		return mat;
	}

	template <typename T>
	Matrix<4, 4, T> lookAt(const Vector<3, float>& eye, const Vector<3, float>& at, const Vector<3, float>& up)
	{
		Matrix<4, 4, T> result;
		Vector<3, float> f = normalize((at - eye));
		Vector<3, float> r = normalize(cross_product(f, up));
		Vector<3, float> u = cross_product(r, f);

		result[0][0] = r.x();
		result[1][0] = r.y();
		result[2][0] = r.z();

		result[0][1] = u.x();
		result[1][1] = u.y();
		result[2][1] = u.z();

		result[0][2] = -f.x();
		result[1][2] = -f.y();
		result[2][2] = -f.z();

		result[3][0] = -r.dot(eye);
		result[3][1] = -u.dot(eye);
		result[3][2] =  f.dot(eye);

		return result.transpose();
	}
}

#endif
