#pragma once

#include <memory>

#include <GameEngineCore/Event.hpp>

namespace GameEngine {

	class Application {
	public:
		Application();
		virtual ~Application() = 0;

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int Start(unsigned int window_width, unsigned int window_height, const char* title);
		virtual void OnUpdate() {}

	private:
		std::unique_ptr<class Window> pwindow_;

		EventDispatcher event_dispatcher_;
		bool bCloseWindow_ = false;
	};

}