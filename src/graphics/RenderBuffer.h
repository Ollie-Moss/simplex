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

struct RenderQueueData {
    glm::vec3 position;
    glm::vec2 size;
    glm::vec4 color;
    std::string texture;
    operator RenderData() const
    {
        return {position, size, color};
    }
};

class RenderBuffer
{
   public:
    RenderBuffer();
    ~RenderBuffer();

    void Insert(const RenderQueueData& data);

    void Render();
    void RenderRange(const size_t& rangeStart, const size_t& rangeEnd);
    void Clear();
    int Size();

   private:
    Buffer m_QuadBuffer;
    Buffer m_InstanceBuffer;

    glm::vec3 m_QuadData[6] = {
        glm::vec3(0.0, 0.0, 0.0),  // bottom left
        glm::vec3(0.0, 1.0, 0.0),  // top left
        glm::vec3(1.0, 1.0, 0.0),  // top right
        glm::vec3(1.0, 0.0, 0.0),  // bottom right
        glm::vec3(0.0, 0.0, 0.0),  // bottom left
        glm::vec3(1.0, 1.0, 0.0)   // top right
    };

    VertexArray m_VertexArray;
    std::array<RenderQueueData, MAX_BUFFER_SIZE> m_RenderData;
    size_t m_Index;
};
