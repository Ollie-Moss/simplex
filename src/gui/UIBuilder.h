#pragma once

#include "core/Entity.h"
#include "core/Registry.h"
#include "gui/UIComponents.h"

struct UISpec {
    UITransform transform;
    UIProperties properties;
    std::vector<UISpec> children;
};

inline UISpec element(UIProperties properties, std::initializer_list<UISpec> children = {})
{
    return {.properties = properties, .children = children};
}

inline std::vector<EntityId> CreateChildrenEntities(Registry &registry, UISpec spec, EntityId parent)
{
    std::vector<EntityId> children;
    for (auto child : spec.children) {
        // Create new entity
        Entity entity = registry.Create<UITransform, UIElement, UIProperties>(child.transform, {.parent = parent}, child.properties);

        // Create children entities
        std::vector<EntityId> newChildren = CreateChildrenEntities(registry, child, entity);
        entity.GetComponent<UIElement>().children = newChildren;

        // Add finalized child entity to list
        children.push_back(entity);
    }
    return children;
}

inline Entity CreateEntityFromUISpec(Registry &registry, UISpec spec)
{
    Entity root = registry.Create<UITransform, UIElement, UIProperties>(spec.transform, {}, spec.properties);
    std::vector<EntityId> newChildren = CreateChildrenEntities(registry, spec, root);
    root.GetComponent<UIElement>().children = newChildren;

    return root;
}
