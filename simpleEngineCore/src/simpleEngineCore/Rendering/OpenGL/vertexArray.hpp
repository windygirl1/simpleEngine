#pragma once
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"

namespace SimpleEngine {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& vertexArray) noexcept;
		VertexArray(VertexArray&& vertexArray) noexcept;

		void addVertexBuffer(const VertexBuffer& vertexBuffer);
		void setIndexBuffer(const IndexBuffer& indexBuffer);
		void bind() const;
		static void unbind();
		size_t getIndicesCount() const { return m_indices_count; }

	private:
		unsigned int m_id = 0;
		unsigned int m_elements_count = 0;
		size_t m_indices_count = 0;
	};
}
