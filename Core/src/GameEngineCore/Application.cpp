#include <GameEngineCore/Application.hpp>
#include <GameEngineCore/Log.hpp>

#include <GLFW/glfw3.h>
#include <iostream>

namespace mge {

	Application::Application() {
		LOG_INFO("Welcome to spdlog!");
		LOG_ERROR("Some error message with arg: {}", 1);

		LOG_WARN("Easy padding in numbers like {:08d}", 12);
		LOG_CRITICAL("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	}

	Application::~Application() {

	}

	int Application::Start(unsigned int window_width, unsigned int window_height, const char* title) {
		GLFWwindow* window;

		if (!glfwInit()) {
			return -1;
		}

		window = glfwCreateWindow(window_width, window_height, title, nullptr, nullptr);
		if (!window) {
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		while (!glfwWindowShouldClose(window)) {
			glfwSwapBuffers(window);
			glfwPollEvents();
			OnUpdate();
		}

		glfwTerminate();
		return 0;
	}
}