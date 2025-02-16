#pragma once

#include "VertexBuffer.hpp"

namespace engine {

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray(VertexArray&& other) noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;

		void AddBuffer(const VertexBuffer& vertex_buffer);

		void Bind() const;
		static void Unbind();
			 
	private:
		unsigned int m_id{};
		unsigned int m_elements_count{};
	};

} // namespace engine