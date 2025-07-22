#pragma once

#include "core/Buffer.h"
#include "core/Sprite.h"
#include "core/VertexArray.h"
#include <array>
#include <map>
#include <set>

class Renderer2D {

  public:
    Renderer2D() = default;
    ~Renderer2D() = default;

    void Queue(Sprite sprite);
    void Render();


  private:
    std::map<std::string, VertexArray> m_VertexArrays;
    std::map<std::string, std::array<Buffer, 3>> m_Buffers;
    std::map<std::string, std::vector<glm::vec3>> m_Positions;
    std::map<std::string, std::vector<glm::vec2>> m_Sizes;
    std::map<std::string, std::vector<glm::vec4>> m_Colors;

    std::set<std::string> m_Textures;
};
