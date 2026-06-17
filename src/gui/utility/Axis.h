#pragma once

#include "gui/utility/SizingMode.h"
#include "gui/utility/SizeValue.h"

struct Axis
{
    Axis() {}
    Axis(SizingMode mode, SizeValue length) : mode(mode), length(length) {}

    bool operator==(const Axis &rhs) const
    {
        return (mode == rhs.mode) && (length == rhs.length);
    }

    SizingMode mode = SizingMode::Hug;
    SizeValue length;
};
