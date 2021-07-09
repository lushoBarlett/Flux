#pragma once

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

struct VertexArray {
	unsigned int m_renderer_id;

	VertexArray() {
		glGenVertexArrays(1, &m_renderer_id);
	}

	~VertexArray() {
		glDeleteVertexArrays(1, &m_renderer_id);
	}

	void bind() const {
		glBindVertexArray(m_renderer_id);
	}

	void unbind() const {
		glBindVertexArray(0);
	}

	void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
		bind();
		vb.bind();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < layout.m_elements.size(); ++i) {
			const auto& element = layout.m_elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
			    i, element.count, element.type, element.normalized, layout.m_stride, (const void*)offset);
			offset += element.count * element.size;
		}
	}
};