#pragma once

#include "core/Event.hpp"

#include <string>
#include <functional>

struct GLFWwindow;

namespace engine {

	class Window {
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(std::string title, unsigned int width, unsigned int height);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void OnUpdate();
		unsigned int GetWidth() const { return m_data.width; }
		unsigned int GetHeight() const { return m_data.height; }

		void SetEventCallback(const EventCallbackFn& callback) {
			m_data.eventCallbackFn = callback;
		}

	private:
		int Init();
		void Shutdown();

	private:
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};

		GLFWwindow* m_window{ nullptr };
		WindowData m_data;
		float m_background_color[4]{ 1.f, 0.f, 0.f, 0.f };
	};

}