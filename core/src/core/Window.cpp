#include "core/Window.hpp"
#include "core/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "core/Rendering/OpenGL/GLSLShaderProgram.hpp"
#include "core/Rendering/OpenGL/VertexBuffer.hpp"
#include "core/Rendering/OpenGL/VertexArray.hpp"

namespace engine {

	static bool is_GLFW_initialized = false;

	GLfloat points[] = {
		0.f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLfloat positions_colors[] = {
		0.f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	const char* vertex_shader =
		"#version 460\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_color;"
		"out vec3 color;"
		"void main() {"
		"	color = vertex_color;"
		"	gl_Position = vec4(vertex_position, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 460\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main() {"
		"	frag_color = vec4(color, 1.0);"
		"}";

	std::unique_ptr<GLSLShaderProgram> shader_program;
	std::unique_ptr<VertexBuffer> points_vbo;
	std::unique_ptr<VertexBuffer> colors_vbo;
	std::unique_ptr<VertexBuffer> positions_colors_vbo;

	std::unique_ptr<VertexArray> vao_2buffers;
	std::unique_ptr<VertexArray> vao_1buffer;


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

		if (!is_GLFW_initialized) {
			if (!glfwInit()) {
				LOG_CRITICAL("Can't initialize GLFW!");
				return -1;
			}
			is_GLFW_initialized = true;
		}

		m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		if (!m_window) {
			LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);
			glfwTerminate();
			return -2;
		}

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG_CRITICAL("Failed to initialize GLAD");
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
				glViewport(0, 0, width, height);
			}
		);

		shader_program = std::make_unique<GLSLShaderProgram>(vertex_shader, fragment_shader);
		if (!shader_program->IsCompiled()) {
			return -1;
		}

		BufferLayout buffer_layout_1vec3{
			ShaderDataType::Float3
		};

		points_vbo = std::make_unique<VertexBuffer>(points, sizeof(points), buffer_layout_1vec3);
		colors_vbo = std::make_unique<VertexBuffer>(colors, sizeof(colors), buffer_layout_1vec3);

		vao_2buffers = std::make_unique<VertexArray>();
		vao_2buffers->AddBuffer(*points_vbo);
		vao_2buffers->AddBuffer(*colors_vbo);

		BufferLayout buffer_layout_2vec3{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};

		vao_1buffer = std::make_unique<VertexArray>();
		positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), buffer_layout_2vec3);
		vao_1buffer->AddBuffer(*positions_colors_vbo);

		return 0;
	}

	void Window::Shutdown() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::OnUpdate() {
		glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
		glClear(GL_COLOR_BUFFER_BIT);

		shader_program->Bind();
		vao_1buffer->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(GetWidth());
		io.DisplaySize.y = static_cast<float>(GetHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGui::ShowDemoWindow();
		static bool use_2_buffers = true;
		ImGui::Checkbox("2 Buffers", &use_2_buffers);

		if (use_2_buffers) {
			shader_program->Bind();
			vao_2buffers->Bind();
		} else {
			shader_program->Bind();
			vao_1buffer->Bind();
		}

		glDrawArrays(GL_TRIANGLES, 0, 3);

		ImGui::Begin("Background color window");
		ImGui::ColorEdit4("Background color", m_background_color);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

}