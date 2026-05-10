#pragma once

#include "core/Types.h"
#include "gui/utility/Sizing.h"
#include "gui/utility/Direction.h"
#include "gui/utility/AlignItems.h"
#include "gui/utility/JustifyContent.h"

// This describes the flex layout properties of a given element
struct UILayout
{
    Bindable<Sizing> sizing;
    Bindable<Direction> direction = Direction::Horizontal;
    Bindable<Padding> padding = Padding(0.0f);
    Bindable<float> gap = 0.0f;

    Bindable<AlignItems> alignItems = AlignItems::Start;
    Bindable<JustifyContent> justifyContent = JustifyContent::Start;
};
