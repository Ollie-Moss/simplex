#pragma once

#include "gui/utility/Unit.h"

struct SizeValue
{
    SizeValue() {}
    SizeValue(float value, Unit unit)
        : value(value), unit(unit)
    {}

    float GetValue() const
    {
        return (unit == Unit::Pixels) ? value : value / 100.0f;
    }

    bool operator==(const SizeValue &rhs) const = default;

    float value = 100.0f;
    Unit unit = Unit::Pixels;
};
