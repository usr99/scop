/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:00:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/20 14:07:43 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.hpp"

namespace fs = std::__fs::filesystem;

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

		if (!glfwInit())
			throw std::runtime_error("GLFW initialization failed");
		
		/* 
		** On MacOS implementation of GLFW, the function glfwInit() changes the working directory
		** so we need to restore it afterwise
		*/
		fs::current_path("..");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // required on Mac
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // no resize
 
		/* Create a window and its OpenGL context */
		GLFWwindow* window;
		window = glfwCreateWindow(WIN_W, WIN_H, "Scop", NULL, NULL);
		if (!window)
			throw std::runtime_error("Window creation failed");

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK)
			throw std::runtime_error("GLEW initialization failed");

		/* Setup ImGui */
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::StyleColorsDark();

		GLCall(glEnable(GL_DEPTH_TEST)); // enable depth buffer
		GLCall(glClearColor(BACKGROUND_COLOR)); // set background color

		/*
		** In OpenGL Core profile, there is no default vertex array object
		** so we must create it
		*/
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		/* Load shaders and .obj model */
		ShaderProgram shader("src/shaders/basic");
		Model object(av[1]);

		renderingLoop(window, object, shader);
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

void renderingLoop(GLFWwindow* window, Model& object, ShaderProgram& shader)
{
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), WIN_W / WIN_H, 0.1f, 50.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.0f, -5.0f));

	float rotate[2] = { 0.0f };
	float translate[3] = { 0.0f };

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Reset transformations */
		object.rotate(-rotate[1], glm::vec3(0.0f, 1.0f, 0.0f));
		object.rotate(-rotate[0], glm::vec3(1.0f, 0.0f, 0.0f));
		object.translate(glm::vec3(-translate[0], -translate[1], -translate[2]));

		/* Create the new ImGui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/* Show space transformations panel */
		ImGui::Begin("Debug panel");
		ImGui::SliderFloat2("Rotate", rotate, 0.0f, 360.0f);
		ImGui::SliderFloat3("Translate", translate, -3.0f, 3.0f);
		ImGui::End();

		object.translate(glm::vec3(translate[0], translate[1], translate[2]));
		object.rotate(rotate[0], glm::vec3(1.0f, 0.0f, 0.0f));
		object.rotate(rotate[1], glm::vec3(0.0f, 1.0f, 0.0f));

		shader.setUniformMat4f("uCamera", proj * view);
		shader.setUniformMat4f("uModel", object.getMatrix());

		object.showSettingsPanel();
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
