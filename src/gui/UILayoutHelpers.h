#pragma once

#include "gui/UILayoutTypes.h"

inline SizeValue Percent(float value) { return SizeValue(value, Unit::Percent); }
inline SizeValue Pixels(float value) { return SizeValue(value, Unit::Percent); }

const Axis GROW = Axis(SizingMode::Grow, Percent(100.0f));
const Axis HUG = Axis(SizingMode::Hug, Pixels(0.0f));

inline glm::vec4 Hex(uint32_t hex)
{
    const float inv255 = 1.0f / 255.0f;

    // If no alpha provided (0xRRGGBB), assume opaque
    if(hex <= 0xFFFFFF)
        hex = (hex << 8) | 0xFF;

    return {
        ((hex >> 24) & 0xFF) * inv255,
        ((hex >> 16) & 0xFF) * inv255,
        ((hex >> 8) & 0xFF) * inv255,
        (hex & 0xFF) * inv255};
}
