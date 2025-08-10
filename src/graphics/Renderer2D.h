#pragma once

#include "core/Types.h"
#include "graphics/Font.h"
#include "graphics/RenderBuffer.h"
#include <optional>
#include <string>

class Renderer2D
{
   public:
    Renderer2D() = default;
    ~Renderer2D() = default;

    void QueueWorldObject(Transform transform, std::string texture = "", Color color = OPAQUE);
    void Render();

    void RenderImmediate(Transform transform, std::string texture = "", Color color = OPAQUE);
    void RenderText(std::string text, glm::vec2 position, glm::vec2 size, glm::vec4 color, Font font);

   private:
    std::optional<RenderBuffer> m_WorldRenderBuffer;
};
