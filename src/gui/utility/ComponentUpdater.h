#pragma once

#include "core/Simplex.h"
#include "core/SimplexModules.h"
#include "core/Registry.h"
#include "core/Types.h"
#include "gui/components/UIElement.h"
#include "gui/utility/IComponentUpdater.h"
#include <functional>

template <typename TComponent>
using ComponentUpdaterFunc = std::function<bool(EntityId, SimplexModules, Registry &)>;

template <typename TProperty>
using PropertyUpdaterFunc = std::function<TProperty(SimplexModules, Registry &)>;

template <typename TComponent>
class ComponentUpdater : public IComponentUpdater
{
  public:
    ComponentUpdater() {}
    ComponentUpdater(ComponentUpdaterFunc<TComponent> updaterFn) : updater(updaterFn) {}
    ~ComponentUpdater() override {}

    template <typename TProperty>
    static ComponentUpdater<TComponent> Create(TProperty TComponent::*memberPointer, const PropertyUpdaterFunc<TProperty> &getter)
    {
        ComponentUpdaterFunc<TComponent> updaterFunc = [memberPointer, getter](EntityId entityId, SimplexModules modules, Registry registry) {
            TComponent &component = registry.GetComponent<TComponent>(entityId);

            TProperty prevValue = component.*memberPointer;
            TProperty newValue = getter(modules, registry);

            component.*memberPointer = newValue;
            bool hasChanged = prevValue != newValue;

            UIElement &uiElement = registry.GetComponent<UIElement>(entityId);
            if(hasChanged)
                uiElement.dirty = true;

            return hasChanged;
        };
        return updaterFunc;
    }

    bool Update(EntityId entity) override
    {
        return updater(entity, Simplex::GetModules(), Simplex::GetRegistry());
    }

  private:
    ComponentUpdaterFunc<TComponent> updater;
};
