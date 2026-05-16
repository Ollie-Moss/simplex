#include "UISpecification.h"
#include "ChildSpecificiation.h"
#include "core/Types.h"
#include "gui/components/UIElement.h"
#include "gui/components/UITransform.h"
#include <memory>
#include <vector>

UISpecification &UISpecification::Children(std::initializer_list<ChildSpecification> children)
{
    m_Children = [children] {
        std::vector<UISpecification> uiSpecificiations;
        for(auto &child : children)
        {
            auto consolidation = child.Consolidate();
            for(auto &uiSpecificiation : consolidation)
            {
                uiSpecificiations.push_back(uiSpecificiation);
            }
        }
        return uiSpecificiations;
    };

    return *this;
}

EntityId UISpecification::build_impl(Registry &registry, EntityId parent)
{
    EntityId selfEntity = build_self(registry, parent);
    auto children = m_Children();

    for(auto &child : children)
    {
        EntityId childEntity = child.build_impl(registry, selfEntity);

        UIElement &element = registry.GetComponent<UIElement>(selfEntity);
        element.children.push_back(childEntity);
    }

    return selfEntity;
}

EntityId UISpecification::build_self(Registry &registry, EntityId parent)
{
    EntityId entity = registry.Create();
    m_Properties.layout.Build(registry, entity);
    m_Properties.style.Build(registry, entity);
    m_Properties.text.Build(registry, entity);
    m_Properties.events.Build(registry, entity);

    registry.AddComponent<UIElement>(entity, UIElement{.id = m_Properties.id, .parent = parent});
    return entity;
}
