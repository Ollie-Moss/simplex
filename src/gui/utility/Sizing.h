#pragma once

#include "gui/utility/Bindable.h"
#include "gui/utility/Axis.h"

struct Sizing
{
    Bindable<Axis> width;
    Bindable<Axis> height;

    bool operator==(const Sizing &) const = default;
};
