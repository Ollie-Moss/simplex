#pragma once

#include <string>
#include <glm/glm.hpp>
#include "graphics/util/VertexArray.h"

class TextRenderer
{
   public:
    TextRenderer();
    void RenderText(std::string text, glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontName);

   private:
    VertexBuffer m_Buffer;
    VertexArray m_VertexArray;
};
