#pragma once

#include <array>
#include <span>
#include "core/Buffer.h"
#include "core/Types.h"
#include "core/VertexArray.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

struct RenderData {
    glm::vec3 position;
    glm::vec2 size;
    glm::vec4 color;
};

class RenderBuffer
{
   public:
    RenderBuffer();
    ~RenderBuffer();

    void Insert(const RenderData& data);
    void BindData(VertexArray* vertexArray);
    void Clear();
    int Size();

    operator Buffer*()
    {
        return &m_Buffer;
    }

   private:
    Buffer m_Buffer;
    std::array<RenderData, MAX_BUFFER_SIZE> m_RenderData;
    size_t m_Index;
};
