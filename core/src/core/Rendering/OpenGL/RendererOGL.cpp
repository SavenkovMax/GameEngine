#include "core/Rendering/OpenGL/RendererOGL.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/Rendering/OpenGL/VertexArray.hpp"
#include "core/Log.hpp"

namespace engine {

	bool RendererOGL::Init(GLFWwindow* window) {
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			LOG_CRITICAL("Failed to initialize GLAD");
			return false;
		}

		LOG_INFO("OpenGL context initialized");
		LOG_INFO("Vendor: {0}", GetVendorStr());
		LOG_INFO("Renderer: {0}", GetRendererStr());
		LOG_INFO("Version: {0}", GetVersionStr());

		return true;
	}

	void RendererOGL::Draw(const VertexArray& vertex_array) {
		vertex_array.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.GetIndicesCount()), GL_UNSIGNED_INT, nullptr);
	}

	void RendererOGL::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void RendererOGL::Clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RendererOGL::SetViewport(unsigned int left_offset, unsigned int bottom_offset, unsigned int width, unsigned int height) {
		glViewport(left_offset, bottom_offset, width, height);
	}

	const char* RendererOGL::GetVendorStr() {
		return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	}

	const char* RendererOGL::GetRendererStr() {
		return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	}

	const char* RendererOGL::GetVersionStr() {
		return reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}

} // namespace engine