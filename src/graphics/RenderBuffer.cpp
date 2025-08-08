#include "RenderBuffer.h"
#include <stdexcept>
#include "core/VertexArray.h"

RenderBuffer::RenderBuffer() {}
RenderBuffer::~RenderBuffer() {}

void RenderBuffer::Insert(const RenderData& renderData)
{
    if (m_Index >= m_RenderData.size()) throw std::out_of_range("RenderBuffer is full");

    size_t insertIndex = 0;
    while (insertIndex < m_Index) {
        const auto& current = m_RenderData[insertIndex];

        if (renderData.position.z > current.position.z || (renderData.position.z == current.position.z && renderData.position.y < current.position.y)) {
            break;
        }

        ++insertIndex;
    }

    for (size_t i = m_Index; i > insertIndex; --i) {
        m_RenderData[i] = m_RenderData[i - 1];
    }

    m_RenderData[insertIndex] = renderData;
    ++m_Index;
}

int RenderBuffer::Size()
{
    return static_cast<int>(m_Index);
}

void RenderBuffer::BindData(VertexArray* vertexArray)
{
    std::vector<RenderData> data(m_RenderData.begin(), m_RenderData.begin() + m_Index);
    m_Buffer.Fill<RenderData>(data);

    vertexArray->Bind<glm::vec3>(0, 0, &m_Buffer);
    vertexArray->Bind<glm::vec2>(1, 3, &m_Buffer);
    vertexArray->Bind<glm::vec4>(2, 5, &m_Buffer);
}

void RenderBuffer::Clear()
{
    m_Index = 0;
}
