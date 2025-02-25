#include "core/Application.hpp"
#include "core/Log.hpp"
#include "core/Window.hpp"
#include "core/Event.hpp"

#include "core/Rendering/OpenGL/GLSLShaderProgram.hpp"
#include "core/Rendering/OpenGL/VertexBuffer.hpp"
#include "core/Rendering/OpenGL/VertexArray.hpp"
#include "core/Rendering/OpenGL/IndexBuffer.hpp"
#include "core/Rendering/OpenGL/RendererOGL.hpp"
#include "core/Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/Modules/UIModule.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <GLFW/glfw3.h>

namespace engine {

	GLfloat positions_colors[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
	};

	GLuint indices[] = {
		0, 1, 2, 3, 2, 1
	};

	const char* vertex_shader =
		R"(#version 460
		layout(location = 0) in vec3 vertex_position;
		layout(location = 1) in vec3 vertex_color;
		uniform mat4 model_matrix;
		uniform mat4 view_projection_matrix;
		out vec3 color;
		void main() {
			color = vertex_color;
			gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
		})";

	const char* fragment_shader =
		R"(#version 460
		in vec3 color;
		out vec4 frag_color;
		void main() {
			frag_color = vec4(color, 1.0);
		})";

	std::unique_ptr<GLSLShaderProgram> shader_program;
	std::unique_ptr<VertexBuffer> positions_colors_vbo;
	std::unique_ptr<IndexBuffer> index_buffer;
	std::unique_ptr<VertexArray> vao;
	float m_background_color[4]{ 0.f, 0.f, 0.f, 0.f };
	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };

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

		m_event_dispatcher.AddEventListener<EventKeyPressed>(
			[&](EventKeyPressed& event) {
				LOG_INFO("[KeyPressed] {0}", static_cast<char>(event.key_code));
			}
		);

		m_event_dispatcher.AddEventListener<EventKeyReleased>(
			[&](EventKeyReleased& event) {
				LOG_INFO("[KeyReleased] {0}", static_cast<char>(event.key_code));
			}
		);

		m_window->SetEventCallback(
			[&](BaseEvent& event) {
				m_event_dispatcher.Dispatch(event);
			}
		);

		// TODO: Scene initialization

		shader_program = std::make_unique<GLSLShaderProgram>(vertex_shader, fragment_shader);
		if (!shader_program->IsCompiled()) {
			return -1;
		}

		BufferLayout buffer_layout_2vec3{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};

		vao = std::make_unique<VertexArray>();
		positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), buffer_layout_2vec3);
		index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
		vao->AddVertexBuffer(*positions_colors_vbo);
		vao->SetIndexBuffer(*index_buffer);

		while (!m_close_window) {
			RendererOGL::SetClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
			RendererOGL::Clear();

			shader_program->Bind();
			glm::mat4 transform = glm::mat4(1.0f);
			glm::vec3 axis(0.0f, 0.f, 1.0f);
			float rotate_in_radians = glm::radians(rotate);
			transform = glm::scale(transform, { scale[0], scale[1], scale[2] });
			transform = glm::rotate(transform, rotate_in_radians, axis);
			transform = glm::translate(transform, {translate[0], translate[1], translate[2]});
			shader_program->SetMatrix4("model_matrix", transform);
			camera.SetPositionRotation(camera_position, camera_rotation);
			camera.SetProjectionMode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
			shader_program->SetMatrix4("view_projection_matrix", camera.GetProjectionMatrix() * camera.GetViewMatrix());

			RendererOGL::Draw(*vao);

			UIModule::OnUIDrawBegin();
			OnUIDraw();
			UIModule::OnUIDrawEnd();

			m_window->OnUpdate();
			OnUpdate();
		}
		m_window = nullptr;

		return 0;
	}
}