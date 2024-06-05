#include <iostream>

#include "GameEngineCore/Utils/test.hpp"
#include <GLFW/glfw3.h>

namespace GameEngine {
	int CheckGLFW() {
		GLFWwindow* window;

		if (!glfwInit()) {
			return -1;
		}

		window = glfwCreateWindow(640, 480, "Hello", nullptr, nullptr);
		if (!window) {
			glfwTerminate();
			return -1;
		}
		
		glfwMakeContextCurrent(window);
		while (!glfwWindowShouldClose(window)) {
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}
}