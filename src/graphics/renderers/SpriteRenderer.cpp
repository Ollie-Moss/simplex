#include "SpriteRenderer.h"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/util/RenderData.h"
#include "graphics/util/RenderSpace.h"
#include <cstddef>
#include <cstdio>
#include "graphics/util/VertexBuffer.h"
#include "core/Simplex.h"
#include "graphics/util/VertexArray.h"
#include "glm/fwd.hpp"

SpriteRenderer::SpriteRenderer()
{
    m_Buffer.reserve(1000);
    m_Projection = Simplex::GetView().CalculateProjection(RenderSpace::Screen);
    m_Shader = Simplex::GetAssetManager().Get<Shader>("SpriteShader");

    // Render with vertex Array
    // Postition at location 0, offset 0
    m_VertexArray.BindProperty<RenderData, glm::vec3>(0, offsetof(RenderData, position) / sizeof(float), &m_InstanceBuffer);
    m_VertexArray.AttributeDivisor(0, 1);

    // Size at location 1, offset 3 float (vec3 pos)
    m_VertexArray.BindProperty<RenderData, glm::vec2>(1, offsetof(RenderData, size) / sizeof(float), &m_InstanceBuffer);
    m_VertexArray.AttributeDivisor(1, 1);

    // Color at location 2, offset 5 floats (vec3 pos, vec2 size)
    m_VertexArray.BindProperty<RenderData, glm::vec4>(2, offsetof(RenderData, color) / sizeof(float), &m_InstanceBuffer);
    m_VertexArray.AttributeDivisor(2, 1);
}

void SpriteRenderer::Submit(const SpriteCommand &data)
{
    m_Buffer.emplace_back(data);
}

void SpriteRenderer::Render()
{
    // Move to vbo
    m_InstanceBuffer.Fill<RenderData>(m_Buffer);

    // set shader
    m_Shader.use();

    // set projection
    m_Shader.setMat4("projection", m_Projection);

    // set texture
    // glActiveTexture(GL_TEXTURE0);
    // Simplex::GetAssetManager().Get<Texture>("atlas").Bind();

    // render
    m_VertexArray.RenderInstanced(6, m_Buffer.size(), GL_TRIANGLES);
    glBindTexture(GL_TEXTURE0, 0);

    m_Buffer.clear();
}
