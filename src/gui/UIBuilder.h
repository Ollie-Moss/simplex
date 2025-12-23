#pragma once

#include "core/Entity.h"
#include "core/Registry.h"
#include "core/Types.h"
#include <vector>
#include "Text.h"
#include "gui/UIBuilderTypes.h"
#include "gui/UIComponents.h"

inline UISpec element(UIProps properties, Bindable<std::vector<UISpec>> children = {})
{
    UISpec spec = UISpec{.properties = properties, .children = children};

    return spec;
}

inline EntityId CreateEntities(Registry &registry, UISpec spec, EntityId parent)
{
    // Create new entity
    Entity entity = registry.QueueCreate<UITransform, UILayout, UIStyle, TextLayout, Text, UIEvents>({}, spec.properties.layout, spec.properties.style, {}, spec.properties.text, spec.properties.events);

    // Create Children
    std::vector<EntityId> children;
    for(auto child : spec.children.Get())
    {
        EntityId childEntity = CreateEntities(registry, child, entity);

        children.push_back(childEntity);
    }

    registry.QueueComponent<UIElement>(entity, {.parent = parent, .children = children, .childrenSpec = spec.children});
    return entity;
}

inline Entity BuildUI(Registry &registry, const UISpec &spec)
{
    EntityId root = CreateEntities(registry, spec, NULL_ENTITY);
    return root;
}
