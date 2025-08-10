#include "Renderer2D.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "graphics/RenderBuffer.h"

void Renderer2D::Queue(ProjectionType projection, Transform transform, std::string texture, Color color) {}

void Renderer2D::QueueWorldObject(Transform transform, std::string texture, Color color)
{
    RenderQueueData data = {.position = transform.position, .size = transform.size, .color = color, .texture = texture};

    if (!m_WorldRenderBuffer.has_value()) {
        m_WorldRenderBuffer.emplace();
    }
    m_WorldRenderBuffer.value().Insert(data);
}

void Renderer2D::QueueUIObject(Transform transform, std::string texture, Color color) {}

void Renderer2D::Render()
{
    m_WorldRenderBuffer.value().Render();
}
