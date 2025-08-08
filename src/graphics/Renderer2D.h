#pragma once

#include "core/Types.h"
#include "core/VertexArray.h"
#include "graphics/RenderBuffer.h"
#include <array>
#include <optional>
#include <string>
#include <unordered_map>


class Renderer2D
{
   public:
    Renderer2D() = default;
    ~Renderer2D() = default;

    void Queue(ProjectionType projection, Transform transform, std::string texture = "", Color color = OPAQUE);
    void Render();

   private:
    std::array<std::optional<RenderBuffer>, MAX_RENDER_BUFFERS> m_RenderBuffers;
    std::array<std::optional<VertexArray>, MAX_RENDER_BUFFERS> m_VertexArrays;

    std::unordered_map<size_t, BufferSeperatedData> m_IndexToBufferSeperatedData;

    std::unordered_map<BufferSeperatedData, size_t> m_BufferSeperatedDataToIndex;

    size_t m_BufferIndex;
};
