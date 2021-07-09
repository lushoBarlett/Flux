#pragma once

#include <vector>
#include <GL/glew.h>

struct VertexBufferElement {
	unsigned int type;
	unsigned int size;
	unsigned int count;
	unsigned char normalized;
};

struct VertexBufferLayout {
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;

	VertexBufferLayout() : m_stride(0) {}

	template<typename T>
	void push(unsigned int count);
};

template<>
void VertexBufferLayout::push<float>(unsigned int count) {
	m_elements.push_back({ GL_FLOAT, sizeof(GLfloat), count, GL_FALSE });
	m_stride += sizeof(GLfloat) * count;
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count) {
	m_elements.push_back({ GL_UNSIGNED_INT, sizeof(GLuint), count, GL_FALSE });
	m_stride += sizeof(GLuint) * count;
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count) {
	m_elements.push_back({ GL_UNSIGNED_BYTE, sizeof(GLbyte), count, GL_TRUE });
	m_stride += sizeof(GLbyte) * count;
}