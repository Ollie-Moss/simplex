#pragma once

#include "core/Registry.h"
#include "core/Types.h"
#include "gui/utility/UIElementProperties.h"
#include <functional>
#include <initializer_list>
#include <vector>
#include <gui/utility/Bindable.h>

class ChildSpecification;

class UISpecification
{
  public:
    UISpecification() {};

    UISpecification &Configure(const UIElementProperties &properties)
    {
        m_Properties = properties;
        return *this;
    }

    UISpecification &Children(std::initializer_list<ChildSpecification> children);

    EntityId Build(Registry &registry)
    {
        return build_impl(registry, NULL_ENTITY);
    }

  public:
  private:
    EntityId build_impl(Registry &registry, EntityId parent);

    EntityId build_self(Registry &registry, EntityId parent);

  private:
    std::function<std::vector<UISpecification>()> m_Children = [] { return std::vector<UISpecification>{}; };
    UIElementProperties m_Properties;
};
