#pragma once

#include <gui/utility/Bindable.h>
#include "gui/components/Text.h"
#include "gui/components/UIEvents.h"
#include "gui/components/UILayout.h"
#include "gui/components/UIStyle.h"
#include "gui/utility/UIComponentDefinition.h"

using UILayoutDefintion = UIComponentDefinition<UILayout>;
using UIStyleDefintion = UIComponentDefinition<UIStyle>;
using TextDefinition = UIComponentDefinition<Text>;
using UIEventDefinition = UIComponentDefinition<UIEvents>;

struct UIElementProperties
{
    std::string id = "N/a";
    UILayoutDefintion layout;
    UIStyleDefintion style;
    TextDefinition text;
    UIEventDefinition events;

    UIElementProperties() = default;

    UIElementProperties(const UIElementProperties &) = default;
    UIElementProperties(UIElementProperties &&) = default;

    UIElementProperties &operator=(const UIElementProperties &) = default;
    UIElementProperties &operator=(UIElementProperties &&) = default;

    // Builder methods
    UIElementProperties &WithId(std::string_view newId)
    {
        id = newId;
        return *this;
    }

    UIElementProperties &WithLayout(const UILayoutDefintion &newLayout)
    {
        layout = newLayout;
        return *this;
    }

    UIElementProperties &WithStyle(const UIStyleDefintion &newStyle)
    {
        style = newStyle;
        return *this;
    }

    UIElementProperties &WithText(const TextDefinition &newText)
    {
        text = newText;
        return *this;
    }

    UIElementProperties &WithEvents(const UIEventDefinition &newEvents)
    {
        events = newEvents;
        return *this;
    }
};
