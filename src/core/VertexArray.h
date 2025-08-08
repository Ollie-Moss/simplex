#pragma once

#include "core/Buffer.h"

class VertexArray
{
   public:
    VertexArray();
    ~VertexArray();

    template <typename T>
    void Bind(int location, Buffer *buffer)
    {
        glBindVertexArray(m_Id);
        glBindBuffer(GL_ARRAY_BUFFER, buffer->ID);
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, sizeof(T) / sizeof(GL_FLOAT), GL_FLOAT, GL_FALSE, 0, (void *)0);
        glVertexAttribDivisor(location, 1);
    }
    template <typename T>
    void Bind(int location, int offset, Buffer *buffer)
    {
        glBindVertexArray(m_Id);
        glBindBuffer(GL_ARRAY_BUFFER, buffer->ID);
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, sizeof(T) / sizeof(GL_FLOAT), GL_FLOAT, GL_FALSE, 0, (GLvoid *)(offset * sizeof(GLfloat)));
        glVertexAttribDivisor(location, 1);
    }

    void Render(GLenum mode = GL_TRIANGLE_STRIP);
    void RenderInstanced(int count, GLenum mode = GL_TRIANGLES);

   private:
    unsigned int m_Id;
    int m_Size = 6;
};
