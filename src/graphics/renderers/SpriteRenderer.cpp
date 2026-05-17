#include "SpriteRenderer.h"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/util/RenderData.h"
#include "graphics/util/RenderMode.h"
#include "graphics/util/RenderSpace.h"
#include "graphics/util/Shader.h"
#include <array>
#include <cstddef>
#include <cstdio>
#include "graphics/util/Texture.h"
#include "graphics/util/VertexBuffer.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "graphics/util/VertexArray.h"
#include "glm/fwd.hpp"

void SpriteRenderer::Submit(const SpriteCommand &data)
{
    if(data.renderSpace == RenderSpace::Screen)
    {
        m_ScreenBuffer.Insert(data);
    }
    else
    {
        m_WorldBuffer.Insert(data);
    }
}

void SpriteRenderer::Render()
{
    RenderBuffer(m_WorldBuffer);
    RenderBuffer(m_ScreenBuffer);
    m_WorldBuffer.Clear();
    m_ScreenBuffer.Clear();
}

void SpriteRenderer::RenderBuffer(Buffer<SpriteCommand> &buffer)
{
    std::vector<std::pair<size_t, size_t>> ranges;
    size_t rangeStart = 0;

    if(buffer.Size() == 0)
        return buffer.Clear();

    for(size_t i = 0; i < buffer.Size(); i++)
    {
        size_t nextIndex = i + 1;
        if(nextIndex >= buffer.Size() || buffer[nextIndex].sprite.texture != buffer[i].sprite.texture)
        {
            // create range
            ranges.push_back({rangeStart, i});
            rangeStart = i + 1;
        }
    }

    // Render ranges
    for(size_t i = 0; i < ranges.size(); i++)
    {
        auto [rangeStart, rangeEnd] = ranges[i];
        RenderRange(buffer, rangeStart, rangeEnd);
    }
}

void SpriteRenderer::RenderRange(const Buffer<SpriteCommand> &buffer, const size_t &rangeStart, const size_t &rangeEnd)
{
    std::vector<RenderData> data;
    data.reserve(buffer.Size());

    for(size_t i = rangeStart; i <= rangeEnd; i++)
    {
        data.emplace_back(buffer[i]);
    }

    // Move to vbo
    m_InstanceBuffer.Fill<RenderData>(data);

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

    // set shader
    Shader shader = Simplex::GetAssetManager().Get<Shader>("SpriteShader");
    shader.use();

    // set projection
    glm::mat4 projection;
    projection = Simplex::GetView().CalculateProjection(buffer[0].renderSpace);
    shader.setMat4("projection", projection);

    // set texture
    // bool useTexture = (texture != "");
    bool useTexture = false;
    shader.setBool("useTexture", useTexture);

    if(useTexture)
    {
        glActiveTexture(GL_TEXTURE0);

        Simplex::GetAssetManager().Get<Texture>("atlas").Bind();
    }

    // render
    int count = rangeEnd + 1 - rangeStart;

    m_VertexArray.RenderInstanced(6, count, GL_TRIANGLES);
    glBindTexture(GL_TEXTURE0, 0);
}
