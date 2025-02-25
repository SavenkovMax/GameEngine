#include "core/Input.hpp"

#include "core/Application.hpp"
#include <GLFW/glfw3.h>

namespace engine {

	void Input::SetWindow(GLFWwindow* window) {
		m_window = window;
	}

	bool Input::IsKeyPressed(KeyCode key_code) {
		auto state = glfwGetKey(m_window, static_cast<int>(key_code));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(MouseCode mouse_code) {
		auto state = glfwGetMouseButton(m_window, static_cast<int>(mouse_code));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition() {
		double xpos, ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);
		return { static_cast<float>(xpos), static_cast<float>(ypos) };
	}

	float Input::GetMouseX() {
		return GetMousePosition().x;
	}

	float Input::GetMouseY() {
		return GetMousePosition().y;
	}

} // namespace engine