#pragma once

// Describes a function with the parameters of Event, type TEvent, and the associated Entity corresponding to the UI element
#include "core/Types.h"
#include <functional>

template <typename TEvent>
using EventHandler = std::function<void(const TEvent &, EntityId)>;

struct ClickEvent
{
    glm::vec2 mousePos;
    int button;
};

struct HoverEnterEvent
{
    glm::vec2 mousePos;
};

struct HoverExitEvent
{
    glm::vec2 mousePos;
};

struct KeyBoardEvent
{
    std::string key;
};

struct ChangeEvent
{
};

struct UIEvents
{
    // EventHandler<ClickEvent> onClick;
    // EventHandler<HoverEnterEvent> onHoverEnter;
    // EventHandler<HoverExitEvent> onHoverExit;
    // EventHandler<KeyBoardEvent> onKeyDown;
    // EventHandler<ChangeEvent> onChange;
    // EventHandler<ChangeEvent> onFocus;
    // EventHandler<ChangeEvent> onBlur;

    bool operator==(const UIEvents &rhs) const = default;
};
