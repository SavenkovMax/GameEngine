#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace engine {

	class Camera {
	public:
		enum class ProjectionMode {
			Perspective,
			Orthographic
		};

		Camera(const glm::vec3& position = {0.f, 0.f, 0.f}, 
			   const glm::vec3& rotation = {0.f, 0.f, 0.f},
			   ProjectionMode projection_mode = ProjectionMode::Perspective);

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
		void SetPositionRotation(const glm::vec3& position, const glm::vec3& rotation);
		void SetProjectionMode(ProjectionMode projection_mode);

		glm::mat4 GetViewMatrix() const { return m_view_matrix; }
		glm::mat4 GetProjectionMatrix() const { return m_projection_matrix; }

		void MoveForward(float delta_forward) {
			m_position += m_direction * delta_forward;
		}
		
		void MoveRight(float delta_right) {
			m_position += m_direction * delta_right;
		}

		void MoveUp(float delta_up) {
			m_position += m_direction * delta_up;
		}

	private:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();

	private:
		glm::mat4 m_view_matrix{};
		glm::mat4 m_projection_matrix{};
		glm::vec3 m_position{};
		glm::vec3 m_rotation{};

		glm::vec3 m_direction{};
		glm::vec3 m_right{};
		glm::vec3 m_up{};

		ProjectionMode m_projection_mode{};
	};

} // namespace engine