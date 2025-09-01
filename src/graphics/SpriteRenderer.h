#pragma once

#include "core/Types.h"
#include "core/VertexArray.h"
#include "graphics/QuadBuffer.h"
#include "graphics/RenderBuffer.h"
#include <array>
#include <cstddef>

class SpriteRenderer
{
  public:
    SpriteRenderer() = default;
    ~SpriteRenderer() = default;

    void Queue(Transform transform, std::string texture = "", Color color = OPAQUE);
    void RenderImmediate(Transform transform, std::string texture = "", Color color = OPAQUE);
    void RenderQueue();

  private:
    void RenderRange(const size_t &rangeStart, const size_t &rangeEnd);
    void Clear();

  private:
    VertexArray m_VertexArray;
    Buffer m_InstanceBuffer;
    QuadBuffer m_QuadBuffer;

    std::array<RenderQueueData, MAX_BUFFER_SIZE> m_RenderData;
    size_t m_Index;
};
