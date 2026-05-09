#pragma once

#include "components/Transform.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "gui/UILayoutTypes.h"
#include "Bindable.h"
#include <functional>
#include <glm/glm.hpp>
#include <vector>

struct UISpecification;

struct UIElement
{
    std::string id = "N/A";
    EntityId parent = NULL_ENTITY;
    std::vector<EntityId> children;
    Bindable<std::vector<UISpecification>> childrenSpec;
    bool toBeDeleted = false;
    bool dirty = true;
};

// This describes the flex layout properties of a given element
struct UILayout
{
    Bindable<Sizing> sizing;
    Bindable<Direction> direction = Direction::Horizontal;
    Bindable<Padding> padding = Padding(0.0f);
    Bindable<float> gap = 0.0f;

    Bindable<AlignItems> alignItems = AlignItems::Start;
    Bindable<JustifyContent> justifyContent = JustifyContent::Start;

    auto bindables()
    {
        return std::tie(
            direction,
            gap,
            justifyContent,
            alignItems,
            padding,
            sizing,
            sizing.Get().width,
            sizing.Get().height);
    }

    bool operator==(const UILayout &rhs) const = default;
};

// This describes the style of a given element that does not effect its final UITransform
struct UIStyle
{
    Bindable<Color> color = TRANSPARENT;

    auto bindables()
    {
        return std::tie(
            color);
    }

    bool operator==(const UIStyle &rhs) const = default;
};

// This component describes the actual rendered dimensions of a given elements
// These values are calculated by the UI layout systems based on the UILayout properties
struct UITransform
{
    glm::vec2 position = glm::vec2(0, 0);
    glm::vec2 size = glm::vec2(10, 10);

    operator Transform()
    {
        return Transform{.position = glm::vec3(position.x, position.y, 0),
                         .size = size};
    }
    bool operator==(const UITransform &rhs) const = default;
};

// Describes a function with the parameters of Event, type TEvent, and the associated Entity corresponding to the UI element
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
    EventHandler<ClickEvent> onClick;
    EventHandler<HoverEnterEvent> onHoverEnter;
    EventHandler<HoverExitEvent> onHoverExit;
    EventHandler<KeyBoardEvent> onKeyDown;
    EventHandler<ChangeEvent> onChange;
    EventHandler<ChangeEvent> onFocus;
    EventHandler<ChangeEvent> onBlur;

    bool operator==(const UIEvents &rhs) const
    {
        return true;
    }
};

struct UIInput
{
    std::string value;

    bool focused = false;
    bool readOnly = false;

    size_t cursor = 0;
    size_t selectionStart = 0;
    size_t selectionEnd = 0;

    bool operator==(const UIInput &) const
    {
        return true;
    }
};
