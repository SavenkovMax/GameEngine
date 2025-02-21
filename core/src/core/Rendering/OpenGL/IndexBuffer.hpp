#pragma once

#include "VertexBuffer.hpp"

namespace engine {

	class IndexBuffer {
	public:
		IndexBuffer(const void* data, size_t count, VertexBuffer::Usage usage = VertexBuffer::Usage::Static);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;
		
		void Bind() const;
		static void Unbind();
		size_t GetCount() const noexcept { return m_count; }


	private:
		unsigned int m_id{};
		size_t m_count{};
	};

} // namespace engine