#pragma once

#include "VertexBuffer.h"
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>

struct QuadVertexBuffer : public VertexBuffer {
    QuadVertexBuffer() : VertexBuffer()
    {
        Fill<glm::vec3>(m_QuadData);
    }

   private:
    glm::vec3 m_QuadData[6] = {
        glm::vec3(0.0, 0.0, 0.0),  // bottom left
        glm::vec3(0.0, 1.0, 0.0),  // top left
        glm::vec3(1.0, 1.0, 0.0),  // top right
        glm::vec3(1.0, 0.0, 0.0),  // bottom right
        glm::vec3(0.0, 0.0, 0.0),  // bottom left
        glm::vec3(1.0, 1.0, 0.0)   // top right
    };
};
