#pragma once

struct GLFWwindow;

namespace engine {
	class VertexArray;

	class RendererOGL {
	public:
		static bool Init(GLFWwindow* window);

		static void Draw(const VertexArray& vertex_array);
		static void SetClearColor(float r, float g, float b, float a);
		static void Clear();
		static void SetViewport(unsigned int left_offset, unsigned int bottom_offset, unsigned int width, unsigned int height);

		static const char* GetVendorStr();
		static const char* GetRendererStr();
		static const char* GetVersionStr();

	private:
	};

} // namespace engine