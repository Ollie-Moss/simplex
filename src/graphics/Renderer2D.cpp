#include "Renderer2D.h"
#include <memory>
#include "core/Simplex.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "graphics/RenderBuffer.h"
#include "graphics/SpriteRenderer.h"
#include "graphics/TextRenderer.h"

bool Renderer2D::Init()
{
    m_WorldRenderBuffer = std::make_unique<RenderBuffer>();
    m_TextRenderer = std::make_unique<TextRenderer>();
    m_SpriteRenderer = std::make_unique<SpriteRenderer>();

    return true;
}
void Renderer2D::QueueWorldObject(Transform transform, std::string texture, Color color)
{
    RenderQueueData data = {.position = transform.position, .size = transform.size, .color = color, .texture = texture};

    m_WorldRenderBuffer->Insert(data);
}
void Renderer2D::QueueUIObject(Transform transform, std::string texture, Color color)
{
    m_SpriteRenderer->Queue(transform, texture, color);
}
void Renderer2D::Render()
{
    m_WorldRenderBuffer->Render();
    m_SpriteRenderer->RenderQueue();
}
void Renderer2D::RenderText(std::string text, glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontName)
{
    m_TextRenderer->RenderText(text, position, size, color, fontName);
}

void Renderer2D::RenderImmediate(Transform transform, std::string texture, Color color)
{
    m_SpriteRenderer->RenderImmediate(transform, texture, color);
}
