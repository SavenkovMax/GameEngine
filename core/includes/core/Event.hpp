#pragma once

#include "KeyCodes.hpp"

#include <functional>
#include <array>

namespace engine {

	enum class EventType : uint8_t {
		WindowResize = 0,
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
		virtual EventType GetType() const = 0;
	};


	class EventDispatcher {
	public:
		template<typename EventType>
		void AddEventListener(std::function<void(EventType&)> callback) {
			auto baseCallback = [func = std::move(callback)](BaseEvent& e) {
				if (e.GetType() == EventType::type) {
					func(static_cast<EventType&>(e));
				}
			};
			m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
		}

		void Dispatch(BaseEvent& event) {
			auto& callback = m_eventCallbacks[static_cast<size_t>(event.GetType())];
			if (callback) {
				callback(event);
			}
		}

	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;
	};


	struct EventMouseMoved : public BaseEvent {
		EventMouseMoved(const double new_x, const double new_y)
			: x(new_x)
			, y(new_y) {
		}

		virtual EventType GetType() const override {
			return type;
		}

		double x;
		double y;

		static const EventType type = EventType::MouseMoved;
	};

	struct EventWindowResize : public BaseEvent {
		EventWindowResize(const unsigned int new_width, const unsigned int new_height)
			: width(new_width)
			, height(new_height) {
		}

		virtual EventType GetType() const override {
			return type;
		}

		unsigned int width;
		unsigned int height;

		static const EventType type = EventType::WindowResize;
	};

	struct EventWindowClose : public BaseEvent {
		virtual EventType GetType() const override {
			return type;
		}

		static const EventType type = EventType::WindowClose;
	};

	struct EventKeyPressed : public BaseEvent {
		EventKeyPressed(KeyCode key_code, bool repeated) : key_code(key_code), repeated(repeated) {

		}

		virtual EventType GetType() const override {
			return type;
		}

		KeyCode key_code;
		bool repeated;
		static const EventType type = EventType::KeyPressed;
	};

	struct EventKeyReleased : public BaseEvent {
		EventKeyReleased(KeyCode key_code) : key_code(key_code) {

		}

		virtual EventType GetType() const override {
			return type;
		}

		KeyCode key_code;
		static const EventType type = EventType::KeyReleased;
	};
}