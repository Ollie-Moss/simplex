#include "VertexArray.h"

#include "glad/glad.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_Id);
    glBindVertexArray(m_Id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::Render(GLenum mode) {
    glBindVertexArray(m_Id);
    glDrawArrays(mode, 0, m_Size);
    glBindVertexArray(0);
}
void VertexArray::RenderInstanced(int count, GLenum mode) {
    glBindVertexArray(m_Id);
    glDrawArraysInstanced(mode, 0, m_Size, count);
    glBindVertexArray(0);
}
