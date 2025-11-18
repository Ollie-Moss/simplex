#pragma once

#include "components/Transform.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>

// clang-format off
enum class AlignItems { Start, End, Center, Stretch };
enum class JustifyContent { Start, End, Center, SpaceBetween, SpaceAround };
enum class FlexDirection { Row, Column };
enum class SizingMode { Fixed, Hug, Grow };
enum class Unit { Pixels, Percent };
// clang-format on
//

struct UIElement
{
    EntityId parent = NULL_ENTITY;
    std::vector<EntityId> children;
    bool dirty = true;
};

struct Length
{
    float value = 100.0f;
    Unit unit = Unit::Pixels;

  public:
    float GetValue()
    {
        return (unit == Unit::Pixels) ? value : value / 100.0f;
    }
};

struct Axis
{
    SizingMode mode = SizingMode::Hug;
    Length length;
};

constexpr Padding operator""_p(long double val)
{
    return {.top = static_cast<float>(val),
            .right = static_cast<float>(val),
            .bottom = static_cast<float>(val),
            .left = static_cast<float>(val)};
}
constexpr Padding operator""_px(long double val)
{
    return {.right = static_cast<float>(val),
            .left = static_cast<float>(val)};
}
constexpr Padding operator""_py(long double val)
{
    return {.top = static_cast<float>(val),
            .bottom = static_cast<float>(val)};
}
constexpr Padding operator""_pr(long double val)
{
    return {.right = static_cast<float>(val)};
}
constexpr Padding operator""_pl(long double val)
{
    return {.left = static_cast<float>(val)};
}
constexpr Padding operator""_pt(long double val)
{
    return {.top = static_cast<float>(val)};
}
constexpr Padding operator""_pb(long double val)
{
    return {.bottom = static_cast<float>(val)};
}

constexpr Axis operator""_percent(long double val)
{
    return {.mode = SizingMode::Fixed, .length = {.value = static_cast<float>(val), .unit = Unit::Percent}};
}
constexpr Axis operator""_pixels(long double val)
{
    return {.mode = SizingMode::Fixed, .length = {.value = static_cast<float>(val)}};
}

const Axis GROW = {.mode = SizingMode::Grow, .length = {.value = 100, .unit = Unit::Percent}};
const Axis HUG = {.mode = SizingMode::Hug, .length = {.value = 0}};

struct Sizing
{
    Axis width;
    Axis height;
};

struct Text
{
    std::string fontName = "Arial";
    std::string content = "";
    float fontSize = 12;
    glm::vec4 color = BLACK;
};

struct UIProperties
{
    Sizing sizing;
    FlexDirection direction = FlexDirection::Row;
    Padding padding;
    Color color = BLUE;
    float gap = 0.0f;
    AlignItems alignItems = AlignItems::Start;
    JustifyContent justifyContent = JustifyContent::Start;

    JustifyContent justifySelf = JustifyContent::Start;
    AlignItems alignSelf = AlignItems::Start;
    Text text;
};

struct UITransform
{
    glm::vec2 position = glm::vec2(0, 0);
    glm::vec2 size = glm::vec2(10, 10);

    operator Transform()
    {
        return Transform{.position = glm::vec3(position.x, position.y, 0),
                         .size = size};
    }
};
