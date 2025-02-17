#pragma once

#include <vector>

namespace engine {

	enum class ShaderDataType {
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4
	};

	struct BufferElement {
		ShaderDataType type;
		uint32_t component_type;
		size_t components_count;
		size_t size;
		size_t offset;

		BufferElement(ShaderDataType type);
	};

	class BufferLayout {
	public:
		BufferLayout(std::initializer_list<BufferElement> elements) 
			: m_elements(std::move(elements)) {
			size_t offset = 0;
			for (auto& element : m_elements) {
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}

		const std::vector<BufferElement>& GetElements() const { return m_elements; }
		size_t GetStride() const { return m_stride; }

	private:
		std::vector<BufferElement> m_elements;
		size_t m_stride{};
	};

	class VertexBuffer {
	public:
		enum class Usage {
			Static,
			Dynamic,
			Stream
		};

		VertexBuffer(const void* data, size_t size, BufferLayout buffer_layout, Usage usage = VertexBuffer::Usage::Static);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		const BufferLayout& GetLayout() const { return m_buffer_layout; }

		void Bind() const;
		static void Unbind();

	private:
		unsigned int m_id{};
		BufferLayout m_buffer_layout;
	};

} // namespace engine