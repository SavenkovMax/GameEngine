#include "VertexBuffer.hpp"
#include "core/Log.hpp"

#include <glad/glad.h>

namespace engine {

	constexpr unsigned int ShaderDataTypeToComponentsCount(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:
			case ShaderDataType::Int:
				return 1;

			case ShaderDataType::Float2:
			case ShaderDataType::Int2:
				return 2;

			case ShaderDataType::Float3:
			case ShaderDataType::Int3:
				return 3;

			case ShaderDataType::Float4:
			case ShaderDataType::Int4:
				return 4;
		}
		LOG_ERROR("Unknown shader data type at [{0} : {1}]", __FILE__, __LINE__);
		return 0;
	}

	constexpr size_t ShaderDataTypeToSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				return sizeof(GLfloat) * ShaderDataTypeToComponentsCount(type);
			
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return sizeof(GLint) * ShaderDataTypeToComponentsCount(type);
		}

		LOG_ERROR("Unknown shader data type at [{0} : {1}]", __FILE__, __LINE__);
		return 0;
	}

	constexpr unsigned int ShaderDataTypeToComponentType(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				return GL_FLOAT;

			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return GL_INT;
		}

		LOG_ERROR("Unknown shader data type at [{0} : {1}]", __FILE__, __LINE__);
		return 0;
	}

	constexpr GLenum UsageToGLenum(VertexBuffer::Usage usage) {
		switch (usage) {
			case VertexBuffer::Usage::Static:  return GL_STATIC_DRAW;
			case VertexBuffer::Usage::Dynamic: return GL_DYNAMIC_DRAW;
			case VertexBuffer::Usage::Stream:  return GL_STREAM_DRAW;
		}

		LOG_ERROR("Unknown vertex buffer usage at [{0} : {1}]", __FILE__, __LINE__);
		return GL_STREAM_DRAW;
	}

	BufferElement::BufferElement(ShaderDataType type)
		: type(type),
		  component_type(ShaderDataTypeToComponentType(type)),
		  components_count(ShaderDataTypeToComponentsCount(type)),
		  size(ShaderDataTypeToSize(type)),
		  offset(0) 
	{

	}

	VertexBuffer::VertexBuffer(const void* data, size_t size, BufferLayout buffer_layout, Usage usage)
		: m_buffer_layout(std::move(buffer_layout))
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, UsageToGLenum(usage));
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
		: m_id(other.m_id),
		  m_buffer_layout(other.m_buffer_layout)
	{
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
