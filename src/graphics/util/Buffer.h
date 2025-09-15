#pragma once

#include "core/Types.h"
#include <array>
#include <iterator>

template <typename T>
class Buffer
{
  public:
    ~Buffer() = default;

    virtual void Insert(const T &data)
    {
        m_BufferData[m_Index++] = data;
    };

    size_t Size() const { return m_Index; }
    void Clear() { m_Index = 0; }

    const std::array<T, MAX_BUFFER_SIZE> &GetRawData() const
    {
        return m_BufferData;
    }

    virtual T &operator[](size_t index) { return m_BufferData[index]; }
    virtual const T &operator[](size_t index) const { return m_BufferData[index]; }

  protected:
    std::array<T, MAX_BUFFER_SIZE> m_BufferData;
    size_t m_Index = 0;
};
