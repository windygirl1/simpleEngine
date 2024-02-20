#pragma once

#include <vector>
namespace SimpleEngine {

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

		BufferElement(const ShaderDataType type);
	};

	class BufferLayout {
	public:
		BufferLayout() : m_elements{}, m_stride{ 0 } {}

		BufferLayout(std::initializer_list<BufferElement> elements) 
			: m_elements(std::move(elements)) {
			size_t offset = 0;
			m_stride = 0;
			for (auto& el : m_elements) {
				el.offset = offset;
				offset += el.size;
				m_stride += el.size;
			}
		}

		const std::vector<BufferElement>& getElements() const { return m_elements; }
		size_t getStride() const { return m_stride; }

	private:
		std::vector<BufferElement> m_elements;
		size_t m_stride = 0;
	};

	class VertexBuffer {
	public:
		enum class EUsage {
			Static,
			Dynamic,
			Stream
		};

		VertexBuffer(const void* data, const size_t size, BufferLayout bufferLayout, const EUsage = VertexBuffer::EUsage::Static);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
		VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

		void bind() const;
		static void unbind();

		const BufferLayout& getLayout() const { return m_buffer_layout; }

	private:
		unsigned int m_id;
		BufferLayout m_buffer_layout;
	};
}