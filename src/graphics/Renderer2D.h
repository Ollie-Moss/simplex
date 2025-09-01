#pragma once

#include "core/Types.h"
#include "graphics/RenderBuffer.h"
#include "graphics/SpriteRenderer.h"
#include "graphics/TextRenderer.h"
#include <memory>
#include <string>

class Renderer2D
{
  public:
    Renderer2D() = default;
    ~Renderer2D() = default;

    bool Init();

    void QueueWorldObject(Transform transform, std::string texture = "", Color color = OPAQUE);
    void QueueUIObject(Transform transform, std::string texture = "", Color color = OPAQUE);
    void Render();

    void RenderImmediate(Transform transform, std::string texture = "", Color color = OPAQUE);
    void RenderText(std::string text, glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontName);

  private:
    std::unique_ptr<RenderBuffer> m_WorldRenderBuffer;
    std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
    std::unique_ptr<TextRenderer> m_TextRenderer;
};
