#include "vertexArray.hpp"
#include "simpleEngineCore/log.hpp"
#include <glad/glad.h>

namespace SimpleEngine {
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_id);
	}
	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept {
		m_id = vertexArray.m_id;
		m_elements_count = vertexArray.m_elements_count;
		vertexArray.m_id = 0;
		vertexArray.m_elements_count = 0;
		return *this;
	}

	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept 
		: m_id(vertexArray.m_id), 
		m_elements_count(vertexArray.m_elements_count) {
		vertexArray.m_id = 0;
		vertexArray.m_elements_count = 0;
	}

	void VertexArray::addBuffer(const VertexBuffer& vertexBuffer) {
		bind();
		vertexBuffer.bind();

		for (const BufferElement& currentElement : vertexBuffer.getLayout().getElements()) {
			glEnableVertexAttribArray(m_elements_count);
			glVertexAttribPointer(
				m_elements_count,
				static_cast<GLint>(currentElement.components_count),
				currentElement.component_type,
				GL_FALSE,
				static_cast<GLsizei>(vertexBuffer.getLayout().getStride()),
				reinterpret_cast<const void*>(currentElement.offset)
			);
			++m_elements_count;
		}
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() {
		glBindVertexArray(0);
	}
}
