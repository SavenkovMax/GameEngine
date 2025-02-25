#include "core/Window.hpp"
#include "core/Log.hpp"
#include "core/Input.hpp"

#include "core/Modules/UIModule.hpp"
#include "core/Rendering/OpenGL/RendererOGL.hpp"

#include <GLFW/glfw3.h>

namespace engine {

	Window::Window(std::string title, unsigned int width, unsigned int height)
		: m_data({ std::move(title), width, height }) {
		int resultCode = Init();
		Input::SetWindow(m_window);
	}

	Window::~Window() {
		UIModule::OnCloseWindow();
		Shutdown();
	}

	int Window::Init() {
		LOG_INFO("Creating window '{0}' with size {1}x{2}", m_data.title, m_data.width, m_data.height);

		glfwSetErrorCallback([](int error_code, const char* desc) {
			LOG_CRITICAL("GLFW error code {0}: {1}", error_code, desc);
		});

		if (!glfwInit()) {
			LOG_CRITICAL("Can't initialize GLFW!");
			return -1;
		}

		m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		if (!m_window) {
			LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);
			return -2;
		}

		if (!RendererOGL::Init(m_window)) {
			LOG_CRITICAL("Failed to initialize OpenGL renderer");
			return -3;
		}

		glfwSetWindowUserPointer(m_window, &m_data);

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action) {
				case GLFW_PRESS: 
				{
					EventKeyPressed event(static_cast<KeyCode>(key), false);
					data.eventCallbackFn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					EventKeyReleased event(static_cast<KeyCode>(key));
					data.eventCallbackFn(event);
					break;
				}
				case GLFW_REPEAT:
				{
					EventKeyPressed event(static_cast<KeyCode>(key), true);
					data.eventCallbackFn(event);
					break;
				}
			}
		});

		glfwSetWindowSizeCallback(m_window,
			[](GLFWwindow* window, int width, int height) {
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				data.width = width;
				data.height = height;

				EventWindowResize event(width, height);
				data.eventCallbackFn(event);
			}
		);

		glfwSetCursorPosCallback(m_window,
			[](GLFWwindow* window, double x, double y) {
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				EventMouseMoved event(x, y);
				data.eventCallbackFn(event);
			}
		);

		glfwSetWindowCloseCallback(m_window,
			[](GLFWwindow* window) {
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				EventWindowClose event;
				data.eventCallbackFn(event);
			}
		);

		glfwSetFramebufferSizeCallback(m_window,
			[](GLFWwindow* window, int width, int height) {
				RendererOGL::SetViewport(0, 0, width, height);
			}
		);

		UIModule::OnCreateWindow(m_window);

		return 0;
	}

	void Window::Shutdown() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::OnUpdate() {
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

}