/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 13:52:45 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/09 20:04:00 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SCOP_HPP_
# define _SCOP_HPP_

# include <iostream>
# include <chrono>

# include "GL/glew.h"
# include "GLFW/glfw3.h"

# include "imgui.h"
# include "imgui_impl_glfw.h"
# include "imgui_impl_opengl3.h"

# include "Model.hpp"
# include "LightSource.hpp"
# include "ArcballCamera.hpp"
# include "debug.hpp"

# define WIN_W 800.0f
# define WIN_H 600.0f

enum ShaderType
{
	MATERIAL,
	REFLECTION,
	REFRACTION
};

struct Settings
{
	using Timestamp = std::chrono::time_point<std::chrono::system_clock>;

	ShaderType current;
	ft::vec3 background;
	int primitive;
	int dotsize;

	bool freeOrbit;
	Timestamp lastRotation;
	
	bool textured;
	float opacity;
	Timestamp lastTransition;
};

void renderingLoop(GLFWwindow* window, const char* objectPath);
long long getDurationFrom(std::chrono::system_clock::time_point& from);
void handleMouseInputs(ArcballCamera& camera, bool isFreeOrbitEnabled);
void handleKeyboardInputs(bool& isTextureEnabled, bool& exitProgram);
void showSettingsWindow(Settings& settings, Model& object, LightSource& light, ArcballCamera& camera);
void handleTime(Settings& settings, Model& object);

#endif
