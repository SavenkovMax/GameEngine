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

	VertexArray::VertexArray(VertexArray&& other) noexcept 
		: m_id(other.m_id),
		  m_elements_count(other.m_elements_count)
	{
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

	void VertexArray::AddVertexBuffer(const VertexBuffer& vertex_buffer) {
		Bind();
		vertex_buffer.Bind();

		for (const BufferElement& curr_element : vertex_buffer.GetLayout().GetElements()) {
			glEnableVertexAttribArray(m_elements_count);
			glVertexAttribPointer(
				m_elements_count,
				static_cast<GLint>(curr_element.components_count),
				curr_element.component_type,
				GL_FALSE,
				static_cast<GLsizei>(vertex_buffer.GetLayout().GetStride()),
				reinterpret_cast<const void*>(curr_element.offset)
			);
			++m_elements_count;
		}
	}

	void VertexArray::SetIndexBuffer(const IndexBuffer& index_buffer) {
		Bind();
		index_buffer.Bind();
		m_indices_count += index_buffer.GetCount();
	}

	void VertexArray::Bind() const {
		glBindVertexArray(m_id);
	}

	void VertexArray::Unbind() {
		glBindVertexArray(0);
	}

} // namespace engine
