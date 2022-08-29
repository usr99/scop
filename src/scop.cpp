/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:00:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/29 13:30:10 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <chrono>

#include "scop.hpp"
#include "LightSource.hpp"
#include "bmp_parser.hpp"

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
	ArcballCamera camera(WIN_W, WIN_H);
	LightSource light;

	bool freeOrbitEnabled = false;
	auto timeLastRotation = std::chrono::system_clock::now();
	auto timeLastTextureFade = std::chrono::system_clock::now();
	float textureOpacity = 0.f;
	bool isTextureEnabled = false;

	BMPimage img("resources/textures/TRU256.bmp");
	unsigned int textureId;
	GLCall(glGenTextures(1, &textureId));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
	GLCall(glActiveTexture(GL_TEXTURE0));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.info.width, img.info.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img.data()));
	shader.setUniform1i("uTexture", 0);

	/* Loop until the user closes the window */
	bool windowShouldClose = false;
	while (!windowShouldClose)
	{
		windowShouldClose = glfwWindowShouldClose(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Create the new ImGui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		handleMouseInputs(camera, freeOrbitEnabled);
		handleKeyboardInputs(isTextureEnabled, windowShouldClose);

		if (!freeOrbitEnabled)
		{
			/* Rotate the model on its Y-axis */
			const float angle = (getDurationFrom(timeLastRotation) * 360.f) / 10000.f; // a full rotation takes 10 seconds
			object.rotate(angle);
		}
		if (isTextureEnabled)
		{
			textureOpacity += getDurationFrom(timeLastTextureFade) / 2000.f;
			textureOpacity = std::min(1.f, textureOpacity);
		}
		else
		{
			textureOpacity -= getDurationFrom(timeLastTextureFade) / 1000.f;
			textureOpacity = std::max(0.f, textureOpacity);
		}

		shader.setUniformMat4f("uCamera", camera.getMatrix());
		shader.setUniformMat4f("uModel", object.getMatrix());
		shader.setUniformVec3f("uLightPosition", light.getPosition());
		shader.setUniform1f("uTextureAlpha", textureOpacity);

		ImGui::Begin("Settings");
		object.showSettingsPanel();
		light.showSettingsPanel();
		if (ImGui::Checkbox("toggle free orbit", &freeOrbitEnabled))
		{
			camera.reset();
			timeLastRotation = std::chrono::system_clock::now(); // avoid big rotations when free orbit is toggled off
		}
		ImGui::End();
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

long long getDurationFrom(std::chrono::system_clock::time_point& from)
{
	const auto now = std::chrono::system_clock::now();
	const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - from).count();
	from = now;
	return elapsedTime;
}

void handleMouseInputs(ArcballCamera& camera, bool isFreeOrbitEnabled)
{
	if (ImGui::IsMousePosValid() && !ImGui::IsAnyItemActive())
	{
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
			if (isFreeOrbitEnabled)
			{
				glm::vec2 angle;
				angle.x = delta.x * (2 * M_PI / WIN_W);
				angle.y = delta.y * (M_PI / WIN_H);
				camera.rotate(angle);
			}
			else
				camera.translate(delta.x / -100.f, delta.y / -100.f); // divide by 100 to slow the movement
			ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
		}
		camera.zoom(ImGui::GetIO().MouseWheel);
	}
}

void handleKeyboardInputs(bool& isTextureEnabled, bool& exitProgram)
{
	if (ImGui::IsKeyDown(ImGuiKey_Escape))
		exitProgram = true;

	if (ImGui::IsKeyPressed(ImGuiKey_T))
		isTextureEnabled = !isTextureEnabled;
}
