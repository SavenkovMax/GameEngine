#include <GameEngineCore/Window.hpp>
#include <GameEngineCore/Log.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GameEngine {

	static bool s_GLFW_initialized = false;

	Window::Window(unsigned int width, unsigned int height, const std::string& title)
		: data_({ title, width, height }) {
		int result_code = Init();
	}

	Window::~Window() {
		ShutDown();
	}

	int Window::Init() {
		LOG_INFO("Window initialization {0} with size {1}x{2}", data_.title, data_.width, data_.height);

		if (!s_GLFW_initialized) {
			if (!glfwInit()) {
				LOG_CRITICAL("Unable to initialize GLFW");
				return -1;
			}
			s_GLFW_initialized = true;
		}

		window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(), nullptr, nullptr);
		if (!window_) {
			LOG_CRITICAL("Failed to create the window {0}", data_.title);
			glfwTerminate();
			return -2;
		}

		glfwMakeContextCurrent(window_);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG_CRITICAL("Failed to initialize GLAD");
			return -3;
		}

		glfwSetWindowUserPointer(window_, &data_);

		glfwSetWindowSizeCallback(window_,
			[](GLFWwindow* handle, int width, int height) {
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(handle));
				data.width = width;
				data.height = height;

				EventWindowResize event(width, height);
				data.EventCallbackFn(event);
			}
		);

		glfwSetCursorPosCallback(window_,
			[](GLFWwindow* handle, double x, double y) {
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(handle));

				EventMouseMoved event(x, y);
				data.EventCallbackFn(event);
			}
		);

		glfwSetWindowCloseCallback(window_,
			[](GLFWwindow* handle) {
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(handle));

				EventWindowClose event;
				data.EventCallbackFn(event);
			}
		);

		return 0;
	}

	void Window::ShutDown() {
		glfwDestroyWindow(window_);
		glfwTerminate();
	}

	void Window::OnUpdate() {
		glClearColor(0, 0, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
}