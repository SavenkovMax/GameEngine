#include "VertexArray.hpp"
#include "core/Log.hpp"

#include <glad/glad.h>

namespace engine {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept {
		m_id = other.m_id;
		m_elements_count = other.m_elements_count;
		other.m_id = 0;
		other.m_elements_count = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
		m_id = other.m_id;
		m_elements_count = other.m_elements_count;
		other.m_id = 0;
		other.m_elements_count = 0;
		return *this;
	}

	void VertexArray::AddBuffer(const VertexBuffer& vertex_buffer) {
		Bind();
		vertex_buffer.Bind();

		glEnableVertexAttribArray(m_elements_count);
		glVertexAttribPointer(m_elements_count, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		++m_elements_count;
	}

	void VertexArray::Bind() const {
		glBindVertexArray(m_id);
	}

	void VertexArray::Unbind() {
		glBindVertexArray(0);
	}

} // namespace engine
