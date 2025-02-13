#pragma once

#include "core/Event.hpp"

#include <memory>

namespace engine {

	class Application {
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int Start(unsigned int window_width, unsigned int window_height, const char* title);

		virtual void OnUpdate() {}

	private:
		std::unique_ptr<class Window> m_window;

		EventDispatcher m_event_dispatcher;
		bool m_close_window = false;
	};

}