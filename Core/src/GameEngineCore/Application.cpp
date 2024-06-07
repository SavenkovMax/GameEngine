#include <GameEngineCore/Application.hpp>
#include <GameEngineCore/Log.hpp>
#include <GameEngineCore/Window.hpp>
#include <GameEngineCore/Event.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace GameEngine {

	Application::Application() {
		LOG_INFO("Starting the application");
	}

	Application::~Application() {
		LOG_INFO("Closing the application");
	}

	int Application::Start(unsigned int width, unsigned int height, const char* title) {
		pwindow_ = std::make_unique<Window>(width, height, title);

		event_dispatcher_.AddEventListener<EventMouseMoved>(
			[](EventMouseMoved& event) {
				LOG_INFO("[MouseMoved] mouse moved to {0}x{1}", event.x, event.y);
			}
		);

		event_dispatcher_.AddEventListener<EventWindowResize>(
			[](EventWindowResize& event) {
				LOG_INFO("[WindowResize] window resized to {0}x{1}", event.width, event.height);
			}
		);

		event_dispatcher_.AddEventListener<EventWindowClose>(
			[&](EventWindowClose& event) {
				LOG_INFO("[WindowClose] window closed");
				bCloseWindow_ = true;
			}
		);

		pwindow_->SetEventCallBack(
			[&](BaseEvent& event) {
				event_dispatcher_.Dispatch(event);
			}
		);

		while (!bCloseWindow_) {
			pwindow_->OnUpdate();
			OnUpdate();
		}

		pwindow_ = nullptr;

		return 0;
	}
}