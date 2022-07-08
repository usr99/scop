/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 04:07:07 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/08 04:10:31 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _DEBUG_HPP_
# define _DEBUG_HPP_

# include <csignal>
# include "GL/glew.h"

/* OpenGL debug */
# define ASSERT(x) if (!(x)) raise(SIGTRAP);
# define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#endif
