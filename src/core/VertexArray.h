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
        glVertexAttribPointer(location, sizeof(T) / sizeof(float), GL_FLOAT, GL_FALSE, 0, (void *)0);
    }
    template <typename T, typename TStride>
    void Bind(int location, int offset, Buffer *buffer)
    {
        glBindVertexArray(m_Id);
        glBindBuffer(GL_ARRAY_BUFFER, buffer->ID);
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, sizeof(T) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(TStride), (void *)(offset * sizeof(float)));
    }

    void AttributeDivisor(int location, int divisor)
    {
        glVertexAttribDivisor(location, divisor);
    }

    void Render(GLenum mode = GL_TRIANGLE_STRIP);
    void Render(int size, GLenum mode = GL_TRIANGLE_STRIP);
    void RenderInstanced(int size, int count, GLenum mode = GL_TRIANGLES);

   private:
    unsigned int m_Id;
    int m_Size = 6;
};
