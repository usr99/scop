/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 10:00:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 20:43:17 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <chrono>

#include "scop.hpp"
#include "BMPimage.hpp"
#include "Skybox.hpp"
#include "textures.hpp"

# if 1

int main(int ac, char **av)
{
	bool error = false;
	bool imguiInitialized = false;

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
		imguiInitialized = true;

		GLCall(glEnable(GL_DEPTH_TEST)); // enable depth buffer
		GLCall(glEnable(GL_BLEND)); // enable blending
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // set blending behavior
		GLCall(glDisable(GL_CULL_FACE)); // disable face culling

		renderingLoop(window, av[1]);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		error = true;
	}

	/* Destroy ImGui context */
	if (imguiInitialized)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}	

	/* Free GLFW */
    glfwTerminate();

	return !error ? EXIT_SUCCESS : EXIT_FAILURE;
}

void renderingLoop(GLFWwindow* window, const char* objectPath)
{
	Model object(objectPath);
	ArcballCamera camera(WIN_W, WIN_H);
	LightSource light;
	Skybox skybox;

	/* Load textures */
	loadTexture(Material::DefaultTexture, 0);
	int textureSamplers[MAX_TEXTURES];
	for (int i = 0; i < MAX_TEXTURES; i++)
		textureSamplers[i] = i;

	/* Load shaders and .obj model */
	ShaderProgram mainShader("src/shaders/object");
	mainShader.setUniformBlock("uMaterials", 0);
	mainShader.setUniform1iv("uTexture[0]", MAX_TEXTURES, textureSamplers);
	
	ShaderProgram reflectionShader("src/shaders/reflection");
	reflectionShader.setUniformBlock("uMaterials", 0);
	reflectionShader.setUniform1i("uCubemap", 0);

	ShaderProgram skyboxShader("src/shaders/skybox");
	skyboxShader.setUniform1i("uCubemap", 0);

	Settings settings = {
		.current = ShaderType::MATERIAL, .background = ft::vec3({ 0.1f, 0.1f, 0.1f }),
		.primitive = GL_TRIANGLES, .dotsize = 1,
		.freeOrbit = false, .lastRotation = std::chrono::system_clock::now(),
		.textured = false, .opacity = 0.f, .lastTransition = std::chrono::system_clock::now()
	};

	/* Loop until the user closes the window */
	bool windowShouldClose = false;
	while (!windowShouldClose)
	{
		windowShouldClose = glfwWindowShouldClose(window);
		GLCall(glClearColor(
			settings.background.x(),
			settings.background.y(),
			settings.background.z(),
			1.f
		)); // set background color
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		/* Create the new ImGui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		handleMouseInputs(camera, settings.freeOrbit);
		handleKeyboardInputs(settings.textured, windowShouldClose);
		handleTime(settings, object);
		showSettingsWindow(settings, object, light, camera);

		if (settings.current > MATERIAL)
		{
			/* Render skybox */
			skyboxShader.bind();
			auto view = camera.getViewMatrix();
			view[0][3] = 0.f; // cancel translation
			view[1][3] = 0.f; // cancel translation
			view[2][3] = 0.f; // cancel translation
			view[3][3] = 1.f; // cancel translation
			skyboxShader.setUniformMat4f("uCamera", camera.getProjectionMatrix() * view);
			skybox.render();

			/* Set reflection uniforms */
			reflectionShader.bind();
			reflectionShader.setUniformMat4f("uCamera", camera.getMatrix());
			reflectionShader.setUniformMat4f("uModel", object.getMatrix());
			reflectionShader.setUniformVec3f("uCameraPosition", camera.getPosition());
			reflectionShader.setUniform1i("uRefractionEnabled", settings.current - 1);
		}
		else
		{
			mainShader.bind();
			mainShader.setUniformMat4f("uCamera", camera.getMatrix());
			mainShader.setUniformMat4f("uModel", object.getMatrix());
			mainShader.setUniformVec3f("uCameraPosition", camera.getPosition());
			mainShader.setUniformVec3f("uLightPosition", light.getPosition());
			mainShader.setUniform1f("uTextureAlpha", settings.opacity);		
		}

		/* Render object */
		object.render(settings.primitive);

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
				ft::vec2 angle;
				angle.x() = delta.x * (2 * M_PI / WIN_W);
				angle.y() = delta.y * (M_PI / WIN_H);
				camera.rotate(angle);
			}
			else
				camera.translate(delta.x / -200.f, delta.y / -200.f); // divide by 200 to slow the movement
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

void showSettingsWindow(Settings& settings, Model& object, LightSource& light, ArcballCamera& camera)
{
	static int tab = 0;
	
	ImGui::SetNextWindowSize(ImVec2(395.f, 140.f));
	ImGui::SetNextWindowPos(ImVec2(5.f, 5.f));
	ImGui::Begin(
		"Settings",
		nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
	);

	static auto showButton = [](const char* label, int index) {
		bool selected = (tab == index);
		if (selected)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.06f, 0.53f, 0.98f, 1.f));
		if (ImGui::Button(label, ImVec2(185,25)))
			tab = index;
		if (selected)
			ImGui::PopStyleColor();
	};

	showButton("Rendering", 0);
	ImGui::SameLine();
	showButton("Scene", 1);
	ImGui::Separator();

	if (tab == 0)
	{
		ImGui::SliderInt(
			"shader presets",
			reinterpret_cast<int *>(&settings.current),
			0, 2, Settings::SHADERS[settings.current]);

		ImGui::RadioButton("triangles", &settings.primitive, GL_TRIANGLES);
		ImGui::SameLine();
		ImGui::RadioButton("dots", &settings.primitive, GL_POINTS);
		ImGui::BeginDisabled(settings.primitive != GL_POINTS);
		if (ImGui::SliderInt("size", &settings.dotsize, 1, 10))
			GLCall(glPointSize(settings.dotsize));
		ImGui::EndDisabled();

		ImGui::BeginDisabled(settings.current != MATERIAL);
		ImGui::ColorEdit3("background", (float *)&settings.background);
		ImGui::EndDisabled();
	}
	else
	{
		object.showSettingsPanel();
		if (ImGui::Checkbox("toggle free orbit", &settings.freeOrbit))
		{
			camera.reset();
			settings.lastRotation = std::chrono::system_clock::now(); // avoid big rotations when free orbit is toggled off
		}
		light.showSettingsPanel();
	}

	ImGui::End();
}

void handleTime(Settings& settings, Model& object)
{
	if (!settings.freeOrbit)
	{
		/* Rotate the model on its Y-axis */
		const float angle = (getDurationFrom(settings.lastRotation) * 360.f) / 10000.f; // a full rotation takes 10 seconds
		object.rotate(angle);
	}
	if (settings.textured)
	{
		settings.opacity += getDurationFrom(settings.lastTransition) / 2000.f;
		settings.opacity = std::min(1.f, settings.opacity);
	}
	else
	{
		settings.opacity -= getDurationFrom(settings.lastTransition) / 2000.f;
		settings.opacity = std::max(0.f, settings.opacity);
	}
}

# else

# include "math.hpp"

void print_vec3(const glm::vec3& v)
{
	std::cout << '[';
	for (int i = 0; i < 3; i++)
		std::cout << v[i] << ", ";
	std::cout << "]\n";	
}

void print_vec4(const glm::vec4& v)
{
	std::cout << '[';
	for (int i = 0; i < 4; i++)
		std::cout << v[i] << ' ';
	std::cout << "]\n";	
}

void print_mat4(const glm::mat4& m)
{
	for (int i = 0; i < 4; i++)
		print_vec4(m[i]);
	std::cout << '\n';
}

int main()
{
	{
		// glm::ft::vec3 a(1, 0, 0);
		// glm::ft::vec3 b = glm::rotate(a, radians(90.f), glm::ft::vec3(0, 1, 0));
		// glm::ft::vec3 c = glm::rotate(a, radians(90.f), glm::ft::vec3(0, 0, 1));

		// glm::ft::mat4 rotA = glm::rotate(radians(90.f), glm::ft::vec3(0.18, 1.25, -0.75));
		// glm::ft::mat4 rotB = glm::rotate(radians(90.f), glm::ft::vec3(3.18, 1.25, -5.75));
		// glm::ft::mat4 rotC = glm::rotate(radians(90.f), glm::ft::vec3(4.18, -1.25, 2.75));

		std::cout << std::setprecision(15);

		print_mat4(glm::lookAt(glm::vec3(0, 0, -5), glm::vec3(), glm::vec3(0, 1, 0)));
		// print_ft::mat4(rotB);
		// print_ft::mat4(rotC);
		// print_ft::vec3(a);
		// print_ft::vec3(b);
		// print_ft::vec3(c);
	}

	{
		// ft::mat4 a = rotate(ft::mat4(), radians(90.f), ft::vec3({0.18, 1.25, -0.75}));
		// ft::mat4 b = rotate(ft::mat4(), radians(90.f), ft::vec3({3.18, 1.25, -5.75}));
		// ft::mat4 c = rotate(ft::mat4(), radians(90.f), ft::vec3({4.18, -1.25, 2.75}));

		// ft::vec3 a({1, 0, 0});
		// ft::vec3 b = rotate(a, radians(90.f), ft::vec3({0, 1, 0}));
		// ft::vec3 c = rotate(a, radians(90.f), ft::vec3({0, 0, 1}));
		// ft::vec3 b = rotateY(90.f) * ft::vec4(a, 1.f);
		// ft::vec3 c = rotateX(90.f) * ft::vec4(b, 1.f);

		std::cout << ft::lookAt<float>(ft::vec3(INIT_POSITION), ft::vec3(), ft::vec3({0, 1, 0})).transpose() << '\n';
		// std::cout << b << '\n';
		// std::cout << c << '\n';

		// std::cout << rotate(radians(90.f), ft::vec3({ 1, 0, 0 })).transpose() << '\n';
		// std::cout << rotate(radians(90.f), ft::vec3({ 0, 1, 0 })).transpose() << '\n';
		// std::cout << rotate(radians(90.f), ft::vec3({ 0, 0, 1 })).transpose() << '\n';
		// std::cout << c << '\n';
	}
}

#endif