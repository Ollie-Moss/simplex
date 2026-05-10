#pragma once

#include "core/Types.h"
#include "gui/utility/Bindable.h"

// This describes the style of a given element that does not effect its final UITransform
struct UIStyle
{
    Bindable<Color> color = TRANSPARENT;

    bool operator==(const UIStyle &rhs) const = default;
};
