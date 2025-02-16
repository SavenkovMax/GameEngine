#include "VertexBuffer.hpp"
#include "core/Log.hpp"

#include <glad/glad.h>

namespace engine {

	GLenum UsageToGLenum(VertexBuffer::Usage usage) {
		switch (usage) {
			case VertexBuffer::Usage::Static:  return GL_STATIC_DRAW;
			case VertexBuffer::Usage::Dynamic: return GL_DYNAMIC_DRAW;
			case VertexBuffer::Usage::Stream:  return GL_STREAM_DRAW;
		}

		LOG_ERROR("Unknown vertex buffer usage");
		return GL_STREAM_DRAW;
	}

	VertexBuffer::VertexBuffer(const void* data, size_t size, Usage usage) {
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, UsageToGLenum(usage));
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept {
		m_id = other.m_id;
		other.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
		m_id = other.m_id;
		other.m_id = 0;
		return *this;
	}

	void VertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



} // namespace engine
