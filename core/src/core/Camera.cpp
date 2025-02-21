#include "core/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {


	Camera::Camera(const glm::vec3& position, 
				   const glm::vec3& rotation, 
				   ProjectionMode projection_mode) 
		: m_position(position),
		  m_rotation(rotation),
		  m_projection_mode(projection_mode)
	{
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void Camera::UpdateViewMatrix() {
		glm::mat4 rotation_matrix_x = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotation_matrix_y = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotation_matrix_z = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotation_matrix = rotation_matrix_x * rotation_matrix_y * rotation_matrix_z;

		glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0f), m_position);

		m_view_matrix = rotation_matrix * translate_matrix;
	}

	void Camera::UpdateProjectionMatrix() {
		if (m_projection_mode == ProjectionMode::Perspective) {

		} else {

		}
	}

	void Camera::SetPosition(const glm::vec3& position) {
		m_position = position;
		UpdateViewMatrix();
	}

	void Camera::SetRotation(const glm::vec3& rotation) {
		m_rotation = rotation;
		UpdateViewMatrix();
	}

	void Camera::SetPositionRotation(const glm::vec3& position, const glm::vec3& rotation) {
		m_position = position;
		m_rotation = rotation;
		UpdateViewMatrix();
	}

	void Camera::SetProjectionMode(ProjectionMode projection_mode) {
		m_projection_mode = projection_mode;
		UpdateProjectionMatrix();
	}

} // namespace engine