#pragma once

#include "core/Entity.h"
#include "core/Registry.h"
#include "core/Types.h"
#include <functional>
#include <vector>
#include "Text.h"
#include "gui/UIBuilderTypes.h"
#include "gui/UIComponents.h"

template <typename... TComponents>
inline UISpecification element(DefaultUIProps properties, TComponents &&...extraComponents, std::function<void(UISpecification &self)> buildChildren = [](UISpecification &) {})
{
    UISpecification spec = UISpecification{.properties = properties};
    spec.extraComponents = std::make_tuple(std::forward<TComponents>(extraComponents)...);
    buildChildren(spec);

    return spec;
}

inline EntityId CreateEntities(Registry &registry, UISpecification spec, EntityId parent)
{
    // Create new entity
    Entity entity = registry.QueueCreate<UITransform, UILayout, UIStyle, TextLayout, Text, UIEvents>({}, spec.properties.layout, spec.properties.style, {}, spec.properties.text, spec.properties.events);

    // Add extraComponents
    std::apply([&](auto &&...components) {
        (registry.QueueComponent(entity, components), ...);
    },
               spec.extraComponents);

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
