/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:00:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/07 13:09:09 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <algorithm>

#include "Model.hpp"

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

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
		if (!window)
			throw std::runtime_error("Window creation failed");

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		// Model object(av[1]);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		error = true;
	}

	/* Free GLFW */
    glfwTerminate();

	return !error ? EXIT_SUCCESS : EXIT_FAILURE;
}
