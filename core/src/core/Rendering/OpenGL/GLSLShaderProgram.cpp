#include "GLSLShaderProgram.hpp"
#include "core/Log.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace engine {

	bool CreateShader(const char* source, GLenum shader_type, GLuint& shader_id) {
		shader_id = glCreateShader(shader_type);
		glShaderSource(shader_id, 1, &source, nullptr);
		glCompileShader(shader_id);

		GLint success;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

		if (!success) {
			char info_log[1024];
			glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);

			LOG_CRITICAL("Shader compilation error\n{}", info_log);
			return false;
		}
		return true;
	}

	GLSLShaderProgram::GLSLShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src) {
		CreateAndLinkProgram(vertex_shader_src, fragment_shader_src);
	}

	GLSLShaderProgram::~GLSLShaderProgram() {
		glDeleteProgram(m_shader_program);
	}

	GLSLShaderProgram::GLSLShaderProgram(GLSLShaderProgram&& other) noexcept {
		m_shader_program = other.m_shader_program;
		other.m_shader_program = 0;
	}

	GLSLShaderProgram& GLSLShaderProgram::operator=(GLSLShaderProgram&& other) noexcept {
		glDeleteProgram(m_shader_program);
		m_shader_program = other.m_shader_program;

		other.m_shader_program = 0;
		return *this;
	}

	void GLSLShaderProgram::CreateAndLinkProgram(const char* vertex_shader_src, const char* fragment_shader_src) {
		GLuint vertex_shader_id = 0;
		if (!CreateShader(vertex_shader_src, GL_VERTEX_SHADER, vertex_shader_id)) {
			LOG_CRITICAL("Vertex shader: compile-time error");
			glDeleteShader(vertex_shader_id);
			return;
		}

		GLuint fragment_shader_id = 0;
		if (!CreateShader(fragment_shader_src, GL_FRAGMENT_SHADER, fragment_shader_id)) {
			LOG_CRITICAL("Fragment shader: compile-time error");
			glDeleteShader(fragment_shader_id);
			return;
		}

		m_shader_program = glCreateProgram();
		glAttachShader(m_shader_program, vertex_shader_id);
		glAttachShader(m_shader_program, fragment_shader_id);
		glLinkProgram(m_shader_program);

		GLint success;
		glGetShaderiv(m_shader_program, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			GLchar info_log[1024];
			glGetProgramInfoLog(m_shader_program, 1024, nullptr, info_log);
			LOG_CRITICAL("Shader program: link-time error\n{0}", info_log);
			glDeleteProgram(m_shader_program);
			m_shader_program = 0;
			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);
			return;
		}

		glDetachShader(m_shader_program, vertex_shader_id);
		glDetachShader(m_shader_program, fragment_shader_id);
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);

		m_is_compiled = true;
	}

	void GLSLShaderProgram::Bind() const {
		glUseProgram(m_shader_program);
	}

	void GLSLShaderProgram::Unbind() {
		glUseProgram(0);
	}

	void GLSLShaderProgram::SetMatrix4(const char* name, const glm::mat4& mat) {
		glUniformMatrix4fv(glGetUniformLocation(m_shader_program, name), 1, GL_FALSE, glm::value_ptr(mat));
	}

} // namespace engine
