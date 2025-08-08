#include "Renderer2D.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

void Renderer2D::Queue(ProjectionType projection, Transform transform, std::string texture, Color color)
{
    RenderData data = {.position = transform.position, .size = transform.size, .color = color};
    BufferSeperatedData bufferSeperatedData = {.texture = texture, .projectionType = projection};

    // Insert into existing buffer
    if (m_BufferSeperatedDataToIndex.find(bufferSeperatedData) != m_BufferSeperatedDataToIndex.end()) {
        size_t index = m_BufferSeperatedDataToIndex[bufferSeperatedData];
        m_RenderBuffers[index].value().Insert(data);
        return;
    }

    // Create new buffer
    if (!m_RenderBuffers[m_BufferIndex].has_value()) {
        m_RenderBuffers[m_BufferIndex].emplace();
    }
    m_RenderBuffers[m_BufferIndex].value().Insert(data);

    m_IndexToBufferSeperatedData.insert({m_BufferIndex, bufferSeperatedData});
    m_BufferSeperatedDataToIndex.insert({bufferSeperatedData, m_BufferIndex});

    m_BufferIndex++;
}

void Renderer2D::Render()
{
    for (size_t i = 0; i < m_BufferIndex; i++) {
        if (!m_VertexArrays[i].has_value()) {
            m_VertexArrays[i].emplace();
        }

        auto &bufferSeperatedData = m_IndexToBufferSeperatedData[i];
        auto &buffer = m_RenderBuffers[i].value();
        auto &vertexArray = m_VertexArrays[i].value();

        buffer.BindData(&vertexArray);

        // set shader and texture
        Shader shader = Simplex::GetResources().GetShader("SpriteShader");
        shader.use();

        glm::mat4 projection;
        if (bufferSeperatedData.projectionType == ProjectionType::WorldSpace) {
            projection = Simplex::GetView().CalculateWorldSpaceProjection();
        } else if (bufferSeperatedData.projectionType == ProjectionType::ScreenSpace) {
            projection = Simplex::GetView().CalculateScreenSpaceProjection();
        }
        shader.setMat4("projection", projection);

        bool useTexture = (bufferSeperatedData.texture != "");
        shader.setBool("useTexture", useTexture);

        if (useTexture) {
            glActiveTexture(GL_TEXTURE0);
            Simplex::GetResources().GetTexture(bufferSeperatedData.texture).Bind();
        }

        vertexArray.RenderInstanced(buffer.Size());
        glBindTexture(GL_TEXTURE0, 0);

        buffer.Clear();
        m_IndexToBufferSeperatedData.erase(i);
        m_BufferSeperatedDataToIndex.erase(bufferSeperatedData);
    }

    m_BufferIndex = 0;
}
