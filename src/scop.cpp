/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:00:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/12 19:44:39 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <algorithm>

#include "Model.hpp"
#include "ShaderProgram.hpp"
#include "debug.hpp"

#define WIN_W 800.0f
#define WIN_H 600.0f

int main(int ac, char **av)
{
	bool error = false;

	try
	{
		/* Check argument validity */
		if (ac != 2)
		{
			throw std::invalid_argument(
				((ac < 2) ? std::string("No argument") : std::string("Too much arguments"))
				 + " provided\nUsage: ./scop [path/to/your/file.obj]"
			);
		}

		GLFWwindow* window;

		/* Initialize the library */
		if (!glfwInit())
			throw std::runtime_error("GLFW initialization failed");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(WIN_W, WIN_H, "Hello World", NULL, NULL);
		if (!window)
			throw std::runtime_error("Window creation failed");

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK)
			throw std::runtime_error("GLEW initialization failed");

		GLCall(glEnable(GL_DEPTH_TEST));

		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		ShaderProgram shader("src/shaders/basic");
		shader.bind();

		/* Setup Dear ImGui context */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		/* Setup Platform/Renderer bindings */
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		/* Setup Dear ImGui style */
		ImGui::StyleColorsDark();

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), WIN_W / WIN_H, 0.1f, 50.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -3.0f));

		Model object(av[1]);
		float rotate[2] = { 0.0f };
		float translate[2] = { 0.0f };

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/* Reset transformations */
			object.rotate(-rotate[1], glm::vec3(0.0f, 1.0f, 0.0f));
			object.rotate(-rotate[0], glm::vec3(1.0f, 0.0f, 0.0f));
			object.translate(glm::vec3(-translate[0], -translate[1], 0.0f));

			/* Create the new ImGui frame */
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Debug panel");
			ImGui::SliderFloat2("Rotate", rotate, 0.0f, 360.0f);
			ImGui::SliderFloat2("Translate", translate, -10.0f, 10.0f);
			ImGui::End();

			/* Apply transformations and render */
			object.translate(glm::vec3(translate[0], translate[1], 0.0f));
			object.rotate(rotate[0], glm::vec3(1.0f, 0.0f, 0.0f));
			object.rotate(rotate[1], glm::vec3(0.0f, 1.0f, 0.0f));
			shader.setUniformMat4f("uMVP", proj * view * object.getMatrix());
			object.render();

			/* Render dear imgui into screen */
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);
			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		error = true;
	}

	/* Destroy ImGui context */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	/* Free GLFW */
    glfwTerminate();

	return !error ? EXIT_SUCCESS : EXIT_FAILURE;
}
