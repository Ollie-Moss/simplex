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

template <template <typename> typename Wrapper = std::type_identity_t>
struct TextBase
{
    std::string fontName = "Arial";
    Wrapper<std::string> content = std::string("");
    float fontSize = 12;
    Color color = BLACK;
    float lineHeight = 1.0f;

    bool operator==(const TextBase &rhs) const = default;
};

using Text = TextBase<>;
using TextDefinition = TextBase<Bindable>;
