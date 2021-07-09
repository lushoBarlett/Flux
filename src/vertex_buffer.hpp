#pragma once

#include <GL/glew.h>

struct VertexBuffer {
	unsigned int m_renderer_id;

	VertexBuffer(const void* data, unsigned int size) {
		glGenBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	void update(const void* data, unsigned int start, unsigned int end) {
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, start, end - start, data);
	}

	void bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	~VertexBuffer() {
		glDeleteBuffers(1, &m_renderer_id);
	}
};