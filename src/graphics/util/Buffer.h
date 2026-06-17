#pragma once

#include "core/Types.h"
#include <vector>

template <typename T>
class Buffer
{
  public:
    ~Buffer() = default;

    virtual void Insert(const T &data)
    {
        m_BufferData.push_back(data);
    }

    size_t Size() const { return m_BufferData.size(); }

    void Clear() { m_BufferData.clear(); }

    const std::vector<T> &GetRawData() const
    {
        return m_BufferData;
    }

    virtual T &operator[](size_t index) { return m_BufferData[index]; }

    virtual const T &operator[](size_t index) const { return m_BufferData[index]; }

  protected:
    std::vector<T> m_BufferData;
};
