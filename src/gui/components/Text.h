#pragma once

#include <gui/utility/Bindable.h>
#include <string>
#include "components/Transform.h"
#include "core/Types.h"
#include "glm/fwd.hpp"

struct GlyphQuad
{
    Transform transform;

    unsigned int texture; // Character::TextureID

    float ascent;
    float descent;
    float advance;
};

struct TextLayout
{
    glm::vec2 size;
    std::vector<GlyphQuad> glyphs;
    std::vector<float> lineWidths;
};

struct Text
{
    std::string fontName = "Arial";
    Bindable<std::string> content = std::string("");
    float fontSize = 12;
    Color color = BLACK;
    float lineHeight = 1.0f;

    bool operator==(const Text &rhs) const = default;
};
