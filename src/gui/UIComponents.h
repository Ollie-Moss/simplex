#pragma once

#include "components/Transform.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <vector>

// clang-format off
enum class AlignItems { Start, End, Center, Stretch };
enum class JustifyContent { Start, End, Center, SpaceBetween, SpaceAround };
enum class FlexDirection { Row, Column };
enum class SizingMode { Fixed, Hug, Grow };
// clang-format on
//

struct UIElement
{
    EntityId parent = NULL_ENTITY;
    std::vector<EntityId> children;
};

struct Axis
{
    SizingMode mode = SizingMode::Hug;
    float length = 0;
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
    return {.mode = SizingMode::Grow, .length = static_cast<float>(val)};
}
constexpr Axis operator""_pixels(long double val)
{
    return {.mode = SizingMode::Fixed, .length = static_cast<float>(val)};
}

const Axis AUTO = {.mode = SizingMode::Grow, .length = 0};

struct Sizing
{
    Axis width;
    Axis height;
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
