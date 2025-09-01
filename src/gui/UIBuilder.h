#pragma once

#include "core/Entity.h"
#include "core/Registry.h"
#include "core/Types.h"
#include "gui/UIComponents.h"
#include <vector>

struct UISpec
{
    UITransform transform;
    UIProperties properties;
    std::vector<UISpec> children;
};

inline UISpec element(UIProperties properties, std::initializer_list<UISpec> children = {})
{
    UISpec spec = {.properties = properties, .children = children};

    if(spec.properties.sizing.width.mode == SizingMode::Fixed)
    {
        spec.transform.size.x = spec.properties.sizing.width.length;
    }
    if(spec.properties.sizing.height.mode == SizingMode::Fixed)
    {
        spec.transform.size.y = spec.properties.sizing.height.length;
    }

    return spec;
}

inline EntityId CreateEntities(Registry &registry, UISpec spec, EntityId parent)
{
    // Create new entity
    Entity entity = registry.Create<UITransform, UIElement, UIProperties>(spec.transform, {.parent = parent}, spec.properties);
    std::vector<EntityId> children;
    for(auto child : spec.children)
    {
        EntityId childEntity = CreateEntities(registry, child, childEntity);

        children.push_back(childEntity);
    }
    entity.GetComponent<UIElement>().children = children;
    return entity;
}

inline Entity CreateEntityFromUISpec(Registry &registry, UISpec spec)
{
    EntityId root = CreateEntities(registry, spec, NULL_ENTITY);
    return root;
}
