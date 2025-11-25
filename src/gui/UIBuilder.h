#pragma once

#include "core/Entity.h"
#include "core/Registry.h"
#include "core/Types.h"
#include "gui/UIComponents.h"
#include <optional>
#include <vector>

struct UIProps
{
    std::optional<UILayout> layout;
    std::optional<UIStyle> style;
    std::optional<UIText> text;
    std::optional<UIBoundText> bindText;
};

struct UISpec
{
    UITransform transform;
    UIProps properties;
    std::vector<UISpec> children;
};

inline UISpec element(UIProps properties, std::initializer_list<UISpec> children = {})
{
    UISpec spec = {.properties = properties, .children = children};

    return spec;
}

inline EntityId CreateEntities(Registry &registry, UISpec spec, EntityId parent)
{
    // Create new entity
    Entity entity = registry.Create<UITransform, UIElement>(spec.transform, {.parent = parent});

    // Add optional styles
    if(spec.properties.layout.has_value())
        registry.AddComponent<UILayout>(entity, *spec.properties.layout);
    else
    {
        registry.AddComponent<UILayout>(entity, UILayout{});
    }

    if(spec.properties.style.has_value())
        registry.AddComponent<UIStyle>(entity, *spec.properties.style);

    if(spec.properties.text.has_value())
        registry.AddComponent<UIText>(entity, *spec.properties.text);

    if(spec.properties.bindText.has_value())
    {
        registry.AddComponent<UIText>(entity, UIText{});
        registry.AddComponent<UIBoundText>(entity, *spec.properties.bindText);
    }

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
