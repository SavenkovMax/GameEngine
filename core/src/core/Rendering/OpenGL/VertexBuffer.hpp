#pragma once

namespace engine {

	class VertexBuffer {
	public:
		enum class Usage {
			Static,
			Dynamic,
			Stream
		};

		VertexBuffer(const void* data, size_t size, Usage usage = VertexBuffer::Usage::Static);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		void Bind() const;
		static void Unbind();

	private:
		unsigned int m_id{};
	};

} // namespace engine