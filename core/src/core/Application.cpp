#include "core/Application.hpp"
#include "core/Log.hpp"
#include "core/Window.hpp"
#include "core/Event.hpp"

#include <iostream>

namespace engine {

	Application::Application() {
		LOG_INFO("Starting Application");
	}


	Application::~Application() {
		LOG_INFO("Closing Application");
	}


	int Application::Start(unsigned int window_width, unsigned int window_height, const char* title) {
		m_window = std::make_unique<Window>(title, window_width, window_height);

		m_event_dispatcher.AddEventListener<EventMouseMoved>(
			[](EventMouseMoved& event) {
				
			});

		m_event_dispatcher.AddEventListener<EventWindowResize>(
			[](EventWindowResize& event) {
				
			});

		m_event_dispatcher.AddEventListener<EventWindowClose>(
			[&](EventWindowClose& event) {
				LOG_INFO("[WindowClose]");
				m_close_window = true;
			});

		m_window->SetEventCallback(
			[&](BaseEvent& event) {
				m_event_dispatcher.Dispatch(event);
			}
		);

		while (!m_close_window) {
			m_window->OnUpdate();
			OnUpdate();
		}
		m_window = nullptr;

		return 0;
	}
}