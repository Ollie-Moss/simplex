#pragma once

#include "core/Types.h"
#include "glm/fwd.hpp"
#include <gui/Text.h>

struct TextCommand
{
    std::vector<GlyphQuad> glyphs;
    glm::vec2 position = glm::vec2(0, 0);
    Color color = BLACK;
};
