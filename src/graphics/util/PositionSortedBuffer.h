#pragma once
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/util/Buffer.h"

class PositionSortedBuffer : public Buffer<SpriteCommand>
{
  public:
    void Insert(const SpriteCommand &data) override
    {
        size_t insertIndex = 0;

        // Find the correct insertion position based on z and y coordinates
        while(insertIndex < m_BufferData.size())
        {
            const auto &current = m_BufferData[insertIndex];
            if(data.transform.position.z > current.transform.position.z ||
               (data.transform.position.z == current.transform.position.z &&
                data.transform.position.y > current.transform.position.y))
            {
                break;
            }
            ++insertIndex;
        }

        // Insert at the correct position
        m_BufferData.insert(m_BufferData.begin() + insertIndex, data);
    }
};
