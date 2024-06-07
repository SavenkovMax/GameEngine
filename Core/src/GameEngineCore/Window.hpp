#pragma once

#include <GameEngineCore/Event.hpp>

#include <string>
#include <functional>

struct GLFWwindow;

namespace GameEngine {

	class Window {
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(unsigned int width, unsigned int height, const std::string& title);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void OnUpdate();

		unsigned int GetWidth() const noexcept { return data_.width; }
		unsigned int GetHeight() const noexcept { return data_.height; }

		void SetEventCallBack(const EventCallbackFn& CallbackFn) {
			data_.EventCallbackFn = CallbackFn;
		}


	private:
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn EventCallbackFn;
		};

		[[nodiscard]] int Init();
		void ShutDown();

		GLFWwindow* window_;
		WindowData data_;
	};

}