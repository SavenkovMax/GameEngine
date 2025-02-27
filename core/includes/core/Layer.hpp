#pragma once

#include <string>

#include "core/Timestep.hpp"
#include "core/Event.hpp"

namespace engine {

	class Layer {
	public:
		Layer(const std::string& layer_name);
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(BaseEvent& event) {}

		const std::string& GetName() const { return m_debug_name; }

	protected:
		std::string m_debug_name;
	};

} // namespace engine