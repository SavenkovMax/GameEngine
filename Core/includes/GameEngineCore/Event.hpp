#pragma once

#include <array>
#include <functional>

namespace GameEngine {

	enum class EventType : unsigned int {
		WindowResize,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		
		EventsCount
	};

	struct BaseEvent {
		virtual ~BaseEvent() = default;
		virtual EventType GetEventType() const = 0;
	};

	class EventDispatcher {
	public:
		template <typename EventType>
		void AddEventListener(std::function<void(EventType&)> callback) {
			auto BaseCallback = [func = std::move(callback)](BaseEvent& e) {
				if (e.GetEventType() == EventType::type) {
					func(static_cast<EventType&>(e));
				}
			};
			event_callbacks_[static_cast<std::size_t>(EventType::type)] = std::move(BaseCallback);
		}

		void Dispatch(BaseEvent& event) {
			auto& callback = event_callbacks_[static_cast<std::size_t>(event.GetEventType())];

			if (callback) {
				callback(event);
			}
		}


	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<std::size_t>(EventType::EventsCount)> event_callbacks_;
	};

	struct EventMouseMoved : public BaseEvent {
		EventMouseMoved(const double new_x, const double new_y)
			: x(new_x), y(new_y) {}

		virtual EventType GetEventType() const override {
			return type;
		}


		double x;
		double y;
		static const EventType type = EventType::MouseMoved;
	};

	struct EventWindowResize : public BaseEvent {
		EventWindowResize(const unsigned int new_width, const unsigned int new_height)
			: width(new_width), height(new_height) {}

		virtual EventType GetEventType() const override {
			return type;
		}

		unsigned int width;
		unsigned int height;
		static const EventType type = EventType::WindowResize;
	};

	struct EventWindowClose : public BaseEvent {
		virtual EventType GetEventType() const override {
			return type;
		}
		static const EventType type = EventType::WindowClose;
	};
}