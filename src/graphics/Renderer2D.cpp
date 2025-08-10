#include "Renderer2D.h"
#include <cstddef>
#include <memory>
#include "core/Simplex.h"
#include "core/Types.h"
#include "core/VertexArray.h"
#include "glm/fwd.hpp"
#include "graphics/QuadBuffer.h"
#include "graphics/RenderBuffer.h"
#include "graphics/Shader.h"
#include "graphics/TextRenderer.h"

bool Renderer2D::Init()
{
    m_WorldRenderBuffer = std::make_unique<RenderBuffer>();
    m_TextRenderer = std::make_unique<TextRenderer>();

    return true;
}
void Renderer2D::QueueWorldObject(Transform transform, std::string texture, Color color)
{
    RenderQueueData data = {.position = transform.position, .size = transform.size, .color = color, .texture = texture};

    m_WorldRenderBuffer->Insert(data);
}
void Renderer2D::Render()
{
    m_WorldRenderBuffer->Render();
}
void Renderer2D::RenderText(std::string text, glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontName)
{
    m_TextRenderer->RenderText(text, position, size, color, fontName);
}

void Renderer2D::RenderImmediate(Transform transform, std::string texture, Color color)
{
    VertexArray vao;
    Buffer vbo;
    QuadBuffer quadBuffer;

    RenderData data = {.position = transform.position, .size = transform.size, .color = color};
    vbo.Fill<RenderData>(data);

    vao.BindProperty<RenderData, glm::vec3>(0, offsetof(RenderData, position) / sizeof(float), &vbo);
    vao.BindProperty<RenderData, glm::vec2>(1, offsetof(RenderData, size) / sizeof(float), &vbo);
    vao.BindProperty<RenderData, glm::vec4>(2, offsetof(RenderData, color) / sizeof(float), &vbo);
    vao.AttributeDivisor(0, 1);
    vao.AttributeDivisor(1, 1);
    vao.AttributeDivisor(2, 1);
    vao.Bind<glm::vec3>(3, &quadBuffer);

    Shader shader = Simplex::GetResources().GetShader("SpriteShader");
    shader.use();

    // set projection
    glm::mat4 projection = Simplex::GetView().CalculateScreenSpaceProjection();
    shader.setMat4("projection", projection);

    // set texture
    bool useTexture = (texture != "");
    shader.setBool("useTexture", useTexture);

    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);
        Simplex::GetResources().GetTexture(texture).Bind();
    }

    vao.RenderInstanced(6, 1);
    glBindTexture(GL_TEXTURE0, 0);
}
