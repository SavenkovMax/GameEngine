#pragma once

// My Game Engine namespace
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
	};

}