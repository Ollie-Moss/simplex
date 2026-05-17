#pragma once

#include "core/Registry.h"
#include "core/Types.h"
#include "gui/utility/ComponentUpdater.h"
#include "gui/utility/IComponentUpdater.h"
#include <memory>
#include <vector>

template <typename TComponent>
struct UIComponentDefinition
{
  public:
    UIComponentDefinition &Configure(const TComponent &componentDefinition)
    {
        m_ComponentDefinition = componentDefinition;
        return *this;
    }

    template <typename TProperty>
    UIComponentDefinition &Bind(TProperty TComponent::*memberPointer, const PropertyUpdaterFunc<TProperty> &getter)
    {
        ComponentUpdater<TComponent> updater = ComponentUpdater<TComponent>::Create(memberPointer, getter);
        m_Updaters.push_back(std::make_shared<ComponentUpdater<TComponent>>(updater));

        return *this;
    }

    void Build(Registry &registry, EntityId entity)
    {
        registry.AddComponent<TComponent>(entity, m_ComponentDefinition);
        for(auto updater : m_Updaters)
        {
            registry.AddUpdater(entity, updater);
        }
    }

  private:
    TComponent m_ComponentDefinition;
    std::vector<std::shared_ptr<IComponentUpdater>> m_Updaters;
};
