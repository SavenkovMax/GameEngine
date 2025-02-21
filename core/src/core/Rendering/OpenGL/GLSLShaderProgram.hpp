#pragma once

#include <glm/glm.hpp>

namespace engine {

	class GLSLShaderProgram {
	public:
		GLSLShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
		~GLSLShaderProgram();

		GLSLShaderProgram(GLSLShaderProgram&& other) noexcept;
		GLSLShaderProgram& operator=(GLSLShaderProgram&& other) noexcept;

		GLSLShaderProgram(const GLSLShaderProgram&) = delete;
		GLSLShaderProgram& operator=(const GLSLShaderProgram&) = delete;

		void Bind() const;
		static void Unbind();

		void SetMatrix4(const char* name, const glm::mat4& mat);

		bool IsCompiled() const noexcept {
			return m_is_compiled;
		}

	private:
		void CreateAndLinkProgram(const char* vertex_shader_src, const char* fragment_shader_src);

	private:
		unsigned int m_shader_program{};
		bool m_is_compiled{ false };
	};

} // namespace engine