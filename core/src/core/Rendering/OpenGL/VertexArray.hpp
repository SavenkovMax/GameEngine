#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace engine {

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray(VertexArray&& other) noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;

		void AddVertexBuffer(const VertexBuffer& vertex_buffer);
		void SetIndexBuffer(const IndexBuffer& index_buffer);

		void Bind() const;
		static void Unbind();

		size_t GetIndicesCount() const noexcept { return m_indices_count; }
			 
	private:
		unsigned int m_id{};
		unsigned int m_elements_count{};
		size_t m_indices_count{};
	};

} // namespace engine