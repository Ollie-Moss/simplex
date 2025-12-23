#pragma once

#include "Bindable.h"

enum class AlignItems {
    Start,
    End,
    Center,
    Stretch,
};

enum class JustifyContent {
    Start,
    End,
    Center,
    SpaceBetween,
    SpaceAround,
};

enum class Direction {
    Horizontal,
    Vertical,
};

enum class SizingMode {
    Fixed,
    Hug,
    Grow,
};

enum class Unit {
    Pixels,
    Percent,
};

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

    bool operator==(const SizeValue &rhs) const
    {
        return GetValue() == rhs.GetValue();
    }

    float value = 100.0f;
    Unit unit = Unit::Pixels;
};

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

struct Sizing
{
    Bindable<Axis> width;
    Bindable<Axis> height;

    bool operator==(const Sizing &rhs) const
    {
        return (width == rhs.width) && (height == rhs.height);
    }
};
