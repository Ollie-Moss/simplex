#pragma once

#include "core/Types.h"
#include "glm/fwd.hpp"
#include <gui/components/Text.h>

struct TextCommand
{
    std::vector<GlyphQuad> glyphs;
    glm::vec2 position = glm::vec2(0, 0);
    Color color = BLACK;
    glm::vec4 clipRect = glm::vec4(0, 0, 0, 0);
};
