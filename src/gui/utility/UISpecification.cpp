#include "UISpecification.h"
#include "ChildSpecificiation.h"
#include "core/Registry.h"
#include "core/SimplexModules.h"
#include "core/Types.h"
#include "gui/components/Text.h"
#include "gui/components/UIElement.h"
#include "gui/components/UITransform.h"
#include "gui/utility/ComponentUpdater.h"
#include <memory>
#include <vector>

UISpecification &UISpecification::Children(const std::initializer_list<ChildSpecification> &children)
{
    m_Children = children;
    return *this;
}

EntityId UISpecification::build_impl(Registry &registry, EntityId parent)
{
    EntityId selfEntity = build_self(registry, parent);
    if(parent == NULL_ENTITY)
        registry.AddComponent<UIRoot>(selfEntity, {});

    if(m_Children.size() <= 0)
        return selfEntity;

    const std::vector<ChildSpecification> &children = m_Children;

    PropertyUpdaterFunc<std::vector<EntityId>> getter = [selfEntity, children](SimplexModules, Registry &registry) {
        std::vector<UISpecification> uiSpecificiations;

        for(const ChildSpecification &child : children)
        {
            const std::vector<UISpecification> &consolidation = child.Consolidate();
            for(const UISpecification &uiSpecificiation : consolidation)
            {
                uiSpecificiations.push_back(uiSpecificiation);
            }
        }
        UIElement &elem = registry.GetComponent<UIElement>(selfEntity);
        std::vector<EntityId> &currentChildren = elem.children;
        for(auto child : currentChildren)
        {
            registry.Destroy(child);
        }

        std::vector<EntityId> childrenEntities;

        for(auto &child : uiSpecificiations)
        {
            EntityId childEntity = child.build_impl(registry, selfEntity);

            childrenEntities.push_back(childEntity);
        }
        return childrenEntities;
    };

    ComponentUpdater<UIElement> updater = ComponentUpdater<UIElement>::Create(&UIElement::children, getter);
    UIElement &elem = registry.GetComponent<UIElement>(selfEntity);
    elem.children = getter(Simplex::GetModules(), registry);

    // registry.AddUpdater(selfEntity, std::make_shared<ComponentUpdater<UIElement>>(updater));

    return selfEntity;
}

EntityId UISpecification::build_self(Registry &registry, EntityId parent)
{
    EntityId entity = registry.Create();
    m_Properties.layout.Build(registry, entity);
    m_Properties.style.Build(registry, entity);
    m_Properties.text.Build(registry, entity);
    m_Properties.events.Build(registry, entity);

    registry.AddComponent<UITransform>(entity, {});
    registry.AddComponent<TextLayout>(entity, {});
    registry.AddComponent<UIElement>(entity, UIElement{.id = m_Properties.id, .parent = parent});
    return entity;
}
