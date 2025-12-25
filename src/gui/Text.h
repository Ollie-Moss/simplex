#pragma once

#include <string>
#include "Bindable.h"
#include "components/Transform.h"
#include "core/Types.h"
#include "glm/fwd.hpp"

struct GlyphQuad
{
    Transform transform;

    unsigned int texture; // Character::TextureID
    size_t charIndex;     // index in original string
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

    auto bindables()
    {
        return std::tie(content);
    }

    bool operator==(const Text &rhs) const = default;
};
