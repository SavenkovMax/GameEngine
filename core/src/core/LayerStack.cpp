#include "core/LayerStack.hpp"

#include <algorithm>

namespace engine {

	LayerStack::LayerStack()  {
		
	}

	LayerStack::~LayerStack() {
		for (Layer* layer : m_layers) {
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_layers.emplace(m_layers.begin() + m_layer_insert_index, layer);
		++m_layer_insert_index;
	}

	void LayerStack::PushOverlay(Layer* layer) {
		m_layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_layer_insert_index, layer);
		if (it != m_layers.begin() + m_layer_insert_index) {
			layer->OnDetach();
			m_layers.erase(it);
			--m_layer_insert_index;
		}
	}

	void LayerStack::PopOverlay(Layer* layer) {
		auto it = std::find(m_layers.begin() + m_layer_insert_index, m_layers.end(), layer);
		if (it != m_layers.end()) {
			layer->OnDetach();
			m_layers.erase(it);
		}
	}

} // namespace engine