#include "IndexBuffer.hpp"
#include "core/Log.hpp"

#include <glad/glad.h>

namespace engine {

	constexpr GLenum UsageToGLenum(VertexBuffer::Usage usage) {
		switch (usage) {
		case VertexBuffer::Usage::Static:  return GL_STATIC_DRAW;
		case VertexBuffer::Usage::Dynamic: return GL_DYNAMIC_DRAW;
		case VertexBuffer::Usage::Stream:  return GL_STREAM_DRAW;
		}

		LOG_ERROR("Unknown vertex buffer usage at [{0} : {1}]", __FILE__, __LINE__);
		return GL_STREAM_DRAW;
	}

	IndexBuffer::IndexBuffer(const void* data, size_t count, VertexBuffer::Usage usage) 
		: m_count(count)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, UsageToGLenum(usage));
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
		: m_id(other.m_id),
		  m_count(other.m_count)
	{
		other.m_id = 0;
		other.m_count = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
		m_id = other.m_id;
		m_count = other.m_count;
		other.m_id = 0;
		other.m_count = 0;

		return *this;
	}

	void IndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} // namespace engine