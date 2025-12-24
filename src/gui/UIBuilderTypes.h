#pragma once

#include "gui/Bindable.h"
#include "gui/Text.h"
#include "gui/UIComponents.h"

struct DefaultUIProps
{
    UILayout layout;
    UIStyle style;
    Text text;
    UIEvents events;

    bool operator==(const DefaultUIProps &rhs) const = default;
};

struct UISpecification
{
    DefaultUIProps properties;
    std::tuple<> extraComponents;

    Bindable<std::vector<UISpecification>> children;

    void AddChild(const UISpecification &spec)
    {
        children.Get().push_back(spec);
    }

    void BindChildren(BindingFunc<std::vector<UISpecification>> &bind)
    {
        children.Set(bind);
    }

    bool operator==(const UISpecification &rhs) const = default;
};
