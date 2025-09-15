#pragma once

#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/util/Buffer.h"

class PositionSortedBuffer : public Buffer<SpriteCommand>
{
  public:
    void Insert(const SpriteCommand &data) override
    {
        assert(m_Index < m_BufferData.size() && "RenderBuffer is full");

        size_t insertIndex = 0;
        while(insertIndex < m_Index)
        {
            const auto &current = m_BufferData[insertIndex];

            if(data.transform.position.z > current.transform.position.z || (data.transform.position.z == current.transform.position.z && data.transform.position.y > current.transform.position.y))
            {
                break;
            }

            ++insertIndex;
        }

        for(size_t i = m_Index; i > insertIndex; --i)
        {
            m_BufferData[i] = m_BufferData[i - 1];
        }

        m_BufferData[insertIndex] = data;
        ++m_Index;
    }
};
