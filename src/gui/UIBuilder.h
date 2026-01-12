#pragma once

#include "core/Entity.h"
#include "core/Registry.h"
#include "core/Types.h"
#include <iostream>
#include <vector>
#include "Text.h"
#include "gui/UIBuilderTypes.h"
#include "gui/UIComponents.h"

inline EntityId CreateEntities(Registry &registry, UISpecification spec, EntityId parent)
{
    // Create new entity
    Entity entity = registry.QueueCreate<UITransform, UILayout, UIStyle, TextLayout, Text, UIEvents>({}, spec.properties.layout, spec.properties.style, {}, spec.properties.text, spec.properties.events);
    for(auto builder : spec.extraComponents)
    {
        builder(entity);
    }

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

inline Entity BuildUI(Registry &registry, const UISpecification &spec)
{
    EntityId root = CreateEntities(registry, spec, NULL_ENTITY);
    return root;
}
