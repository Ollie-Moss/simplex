#pragma once

#include "core/Entity.h"
#include "core/Registry.h"
#include "core/Types.h"
#include "gui/UIComponents.h"
#include <initializer_list>
#include <optional>
#include <vector>

struct UIProps
{
    std::optional<UILayout> layout;
    std::optional<UIStyle> style;
    std::optional<Text> text;
};

struct UISpec
{
    UIProps properties;
    std::vector<UISpec> children;
};

inline UISpec element(UIProps properties, std::initializer_list<UISpec> children = {})
{
    UISpec spec = UISpec{.properties = properties, .children = children};

    return spec;
}

inline EntityId CreateEntities(Registry &registry, UISpec spec, EntityId parent)
{
    // Create new entity
    Entity entity = registry.Create<UITransform, UIElement>({}, {.parent = parent});

    UILayout layout = spec.properties.layout.has_value() ? *spec.properties.layout : UILayout{};
    registry.AddComponent<UILayout>(entity, layout);

    UIStyle style = spec.properties.style.has_value() ? *spec.properties.style : UIStyle{};
    registry.AddComponent<UIStyle>(entity, style);

    Text text = spec.properties.text.has_value() ? *spec.properties.text : Text{};
    registry.AddComponent<Text>(entity, text);

    // Create Children
    std::vector<EntityId> children;
    for(auto child : spec.children)
    {
        EntityId childEntity = CreateEntities(registry, child, entity);

        children.push_back(childEntity);
    }

    entity.GetComponent<UIElement>().children = children;
    return entity;
}

inline Entity BuildUI(Registry &registry, const UISpec &spec)
{
    EntityId root = CreateEntities(registry, spec, NULL_ENTITY);
    return root;
}
