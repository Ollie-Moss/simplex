#pragma once

#include "core/Types.h"

// This describes the style of a given element that does not effect its final UITransform
struct UIStyle
{
    Color color = TRANSPARENT;

    bool operator==(const UIStyle &rhs) const = default;
};
