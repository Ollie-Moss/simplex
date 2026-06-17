#pragma once

#include "gui/utility/Axis.h"

struct Sizing
{
    Axis width;
    Axis height;

    bool operator==(const Sizing &) const = default;
};
