#pragma once

#include "glad/glad.h"
#include <cstddef>
#include <initializer_list>
#include <span>
#include <vector>

struct Buffer {
    unsigned int ID;

    Buffer()
    {
        glGenBuffers(1, &ID);
    }

    template <typename T>
    Buffer(std::initializer_list<T> buf) : Buffer()
    {
        Fill<T>(buf);
    }

    ~Buffer()
    {
        glDeleteBuffers(1, &ID);
    }

    template <typename T>
    void Fill(std::vector<T> buf)
    {
        Fill(buf.size(), &buf[0]);
    }
    template <typename T>
    void Fill(std::span<T> buf)
    {
        Fill(buf.size(), &buf[0]);
    }
    template <typename T>
    void Fill(size_t size, T *data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    template <typename T>
    void Fill(T value)
    {
        Fill(1, &value);
    }
};
