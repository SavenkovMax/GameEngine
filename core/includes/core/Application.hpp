#pragma once

#include "core/Core.hpp"
#include "core/Event.hpp"
#include "core/Camera.hpp"
#include "core/LayerStack.hpp"
#include "core/Layer.hpp"
#include <glm/glm.hpp>
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

		virtual int Run(unsigned int window_width, unsigned int window_height, const char* title);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		virtual void OnUpdate() {}
		virtual void OnUIDraw() {}

		glm::vec3 camera_position = { 0.f, 0.f, -1.0f };
		glm::vec3 camera_rotation = { 0.f, 0.f, 0.f };
		bool perspective_camera = true;
		Camera camera;

	private:
		std::unique_ptr<class Window> m_window;
		float m_last_frame_time{ 0.f };

		LayerStack m_layer_stack{};
		EventDispatcher m_event_dispatcher;

		bool m_is_running{ true };
		bool m_is_window_minimized{ false };
	};

}