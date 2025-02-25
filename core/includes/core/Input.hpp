#pragma once

#include "core/KeyCodes.hpp"
#include "core/MouseCodes.hpp"
#include <glm/glm.hpp>
#include <array>

struct GLFWwindow;

namespace engine {

	class Input {
	public:
		static void SetWindow(GLFWwindow* window);

		static bool IsKeyPressed(KeyCode key_code);
		static bool IsMouseButtonPressed(MouseCode mouse_code);

		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	private:
		static inline GLFWwindow* m_window = nullptr;
	};

} // namespace engine