#include "core/Window.hpp"
#include "core/Log.hpp"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "core/Rendering/OpenGL/GLSLShaderProgram.hpp"
#include "core/Rendering/OpenGL/VertexBuffer.hpp"
#include "core/Rendering/OpenGL/VertexArray.hpp"
#include "core/Rendering/OpenGL/IndexBuffer.hpp"
#include "core/Camera.hpp"
#include "core/Rendering/OpenGL/RendererOGL.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	glm::vec3 scale_factors(0.5f, 1.5f, 1.0f);
	float rotate = 0.f;
	glm::vec3 translate(0.f, 0.f, 1.f);

	glm::vec3 camera_position(0.0f, 0.0f, 1.0f);
	glm::vec3 camera_rotation(0.f, 0.f, 0.f);
	bool perspective_camera = false;
	Camera camera;


	Window::Window(std::string title, unsigned int width, unsigned int height)
		: m_data({ std::move(title), width, height }) {
		int resultCode = Init();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	}

	Window::~Window() {
		Shutdown();
	}

	int Window::Init() {
		LOG_INFO("Creating window '{0}' with size {1}x{2}", m_data.title, m_data.width, m_data.height);

		glfwSetErrorCallback([](int error_code, const char* desc) {
			LOG_CRITICAL("GLFW error code {0}: {1}", error_code, desc);
		});

		if (!glfwInit()) {
			LOG_CRITICAL("Can't initialize GLFW!");
			return -1;
		}

		m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		if (!m_window) {
			LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);
			return -2;
		}

		if (!RendererOGL::Init(m_window)) {
			LOG_CRITICAL("Failed to initialize OpenGL renderer");
			return -3;
		}

		glfwSetWindowUserPointer(m_window, &m_data);

		glfwSetWindowSizeCallback(m_window,
			[](GLFWwindow* pWindow, int width, int height) {
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
				data.width = width;
				data.height = height;

				EventWindowResize event(width, height);
				data.eventCallbackFn(event);
			}
		);

		glfwSetCursorPosCallback(m_window,
			[](GLFWwindow* pWindow, double x, double y) {
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

				EventMouseMoved event(x, y);
				data.eventCallbackFn(event);
			}
		);

		glfwSetWindowCloseCallback(m_window,
			[](GLFWwindow* pWindow) {
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

				EventWindowClose event;
				data.eventCallbackFn(event);
			}
		);

		glfwSetFramebufferSizeCallback(m_window,
			[](GLFWwindow* pWindow, int width, int height) {
				RendererOGL::SetViewport(0, 0, width, height);
			}
		);

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

		return 0;
	}

	void Window::Shutdown() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::OnUpdate() {
		RendererOGL::SetClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
		RendererOGL::Clear();

		shader_program->Bind();
		glm::mat4 transform = glm::mat4(1.0f);
		glm::vec3 axis(0.0f, 0.f, 1.0f);
		float rotate_in_radians = glm::radians(rotate);
		transform = glm::scale(transform, scale_factors);
		transform = glm::rotate(transform, rotate_in_radians, axis);
		transform = glm::translate(transform, translate);
		shader_program->SetMatrix4("model_matrix", transform);
		camera.SetPositionRotation(camera_position, camera_rotation);
		camera.SetProjectionMode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
		shader_program->SetMatrix4("view_projection_matrix", camera.GetProjectionMatrix() * camera.GetViewMatrix());

		RendererOGL::Draw(*vao);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(GetWidth());
		io.DisplaySize.y = static_cast<float>(GetHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGui::ShowDemoWindow();

		ImGui::Begin("Background color window");
		ImGui::ColorEdit4("Background color", m_background_color);
		ImGui::SliderFloat3("Scale", glm::value_ptr(scale_factors), 0.f, 2.0f);
		ImGui::SliderFloat("Rotate", &rotate, 0.f, 360.f);
		ImGui::SliderFloat3("Translate", glm::value_ptr(translate), -1.f, 1.f);
		ImGui::SliderFloat3("Camera position", glm::value_ptr(camera_position), -10.f, 10.f);
		ImGui::SliderFloat3("Camera rotation", glm::value_ptr(camera_rotation), 0.f, 360.f);
		ImGui::Checkbox("Perspective", &perspective_camera);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

}