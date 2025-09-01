#include "SpriteRenderer.h"
#include "graphics/Shader.h"
#include <array>
#include <cstddef>
#include <cstdio>
#include "core/Buffer.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "core/VertexArray.h"
#include "glm/fwd.hpp"

void SpriteRenderer::Queue(Transform transform, std::string texture, Color color)
{
    RenderQueueData data = {.position = transform.position, .size = transform.size, .color = color, .texture = texture};
    m_RenderData[m_Index] = data;
    m_Index++;
}
void SpriteRenderer::RenderImmediate(Transform transform, std::string texture, Color color)
{
    RenderData data = {.position = transform.position, .size = transform.size, .color = color};
    m_InstanceBuffer.Fill<RenderData>(data);

    m_VertexArray.BindProperty<RenderData, glm::vec3>(0, offsetof(RenderData, position) / sizeof(float), &m_InstanceBuffer);
    m_VertexArray.BindProperty<RenderData, glm::vec2>(1, offsetof(RenderData, size) / sizeof(float), &m_InstanceBuffer);
    m_VertexArray.BindProperty<RenderData, glm::vec4>(2, offsetof(RenderData, color) / sizeof(float), &m_InstanceBuffer);
    m_VertexArray.AttributeDivisor(0, 1);
    m_VertexArray.AttributeDivisor(1, 1);
    m_VertexArray.AttributeDivisor(2, 1);
    m_VertexArray.Bind<glm::vec3>(3, &m_QuadBuffer);

    Shader shader = Simplex::GetResources().GetShader("SpriteShader");
    shader.use();

    // set projection
    glm::mat4 projection = Simplex::GetView().CalculateScreenSpaceProjection();
    shader.setMat4("projection", projection);

    // set texture
    bool useTexture = (texture != "");
    shader.setBool("useTexture", useTexture);

    if(useTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        Simplex::GetResources().GetTexture(texture).Bind();
    }

    m_VertexArray.RenderInstanced(6, 1);
    glBindTexture(GL_TEXTURE0, 0);
}

void SpriteRenderer::RenderQueue()
{
    // Create Ranges
    std::array<std::pair<size_t, size_t>, MAX_BUFFER_SIZE> ranges;
    size_t rangeIndex = 0;

    size_t rangeStart = 0;
    if(m_Index == 0)
        return Clear();

    for(size_t i = 0; i < m_Index; i++)
    {
        size_t nextIndex = i + 1;
        if(nextIndex >= m_Index || m_RenderData[nextIndex].texture != m_RenderData[i].texture)
        {
            // create range
            std::pair<size_t, size_t> range = {rangeStart, i};
            ranges[rangeIndex] = range;
            rangeIndex++;

            rangeStart = i + 1;
        }
    }
    // Render ranges
    for(size_t i = 0; i < rangeIndex; i++)
    {
        auto [rangeStart, rangeEnd] = ranges[i];
        RenderRange(rangeStart, rangeEnd);
    }
    Clear();
}
void SpriteRenderer::RenderRange(const size_t &rangeStart, const size_t &rangeEnd)
{
    std::string texture = m_RenderData[rangeStart].texture;
    std::vector<RenderData> data(m_RenderData.begin() + rangeStart, m_RenderData.begin() + rangeEnd + 1);

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

    m_VertexArray.Bind<glm::vec3>(3, &m_QuadBuffer);

    // set shader
    Shader shader = Simplex::GetResources().GetShader("SpriteShader");
    shader.use();

    // set projection
    glm::mat4 projection;
    projection = Simplex::GetView().CalculateScreenSpaceProjection();
    shader.setMat4("projection", projection);

    // set texture
    bool useTexture = (texture != "");
    shader.setBool("useTexture", useTexture);

    if(useTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        Simplex::GetResources().GetTexture(texture).Bind();
    }

    // render
    int count = rangeEnd + 1 - rangeStart;

    m_VertexArray.RenderInstanced(6, count, GL_TRIANGLES);
    glBindTexture(GL_TEXTURE0, 0);

}

void SpriteRenderer::Clear()
{
    m_Index = 0;
}
