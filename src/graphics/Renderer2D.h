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
    void QueueWorldObject(Transform transform, std::string texture = "", Color color = OPAQUE);
    void QueueUIObject(Transform transform, std::string texture = "", Color color = OPAQUE);
    void Render();

   private:
    std::optional<RenderBuffer> m_WorldRenderBuffer;
};
