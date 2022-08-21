/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 13:52:45 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/21 19:56:10 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SCOP_HPP_
# define _SCOP_HPP_

# include <iostream>
# include <filesystem>

# include "GL/glew.h"
# include "GLFW/glfw3.h"

# include "imgui/imgui.h"
# include "imgui/imgui_impl_glfw.h"
# include "imgui/imgui_impl_opengl3.h"

# include "Model.hpp"
# include "ShaderProgram.hpp"
# include "ArcballCamera.hpp"
# include "debug.hpp"

# define WIN_W 800.0f
# define WIN_H 600.0f
# define BACKGROUND_COLOR 0.404f, 0.631f, 0.624f, 1.0f

void renderingLoop(GLFWwindow* window, Model& object, ShaderProgram& shader);
void handleMouseInputs(ArcballCamera& camera);
void handleKeyboardInputs(bool* exitProgram);

#endif
