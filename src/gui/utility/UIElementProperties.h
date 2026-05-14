#pragma once

#include <gui/utility/Bindable.h>
#include "gui/components/Text.h"
#include "gui/components/UIEvents.h"
#include "gui/components/UILayout.h"
#include "gui/components/UIStyle.h"

struct UIElementProperties
{
    std::string id = "N/a";
    UILayout layout;
    UIStyle style;
    TextDefinition text;
    UIEvents events;

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

    UIElementProperties &WithLayout(const UILayoutDefinition &newLayout)
    {
        layout = newLayout;
        return *this;
    }

    UIElementProperties &WithStyle(const UIStyleDefinition &newStyle)
    {
        style = newStyle;
        return *this;
    }

    UIElementProperties &WithText(const TextDefinition &newText)
    {
        text = newText;
        return *this;
    }

    UIElementProperties &WithEvents(const UIEventsDefinition &newEvents)
    {
        events = newEvents;
        return *this;
    }
};
