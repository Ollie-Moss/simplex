#pragma once

#include "gui/Text.h"
#include "gui/UIComponents.h"

struct UIProps
{
    UILayout layout;
    UIStyle style;
    Text text;
    UIEvents events;

    bool operator==(const UIProps &rhs) const = default;
};

struct UISpec
{
    UIProps properties;
    Bindable<std::vector<UISpec>> children;

    bool operator==(const UISpec &rhs) const = default;
};
