#pragma once

#include "core/Types.h"
#include "core/Registry.h"
#include "gui/utility/UIElementProperties.h"
#include <initializer_list>
#include <gui/utility/Bindable.h>
#include <vector>

class ChildSpecification;

struct UIRoot
{
};

class UISpecification
{
  public:
    UISpecification() = default;
    UISpecification(const UISpecification &) = default;
    UISpecification(UISpecification &&) = default;

    UISpecification &operator=(const UISpecification &) = default;
    UISpecification &operator=(UISpecification &&) = default;

    UISpecification &Configure(const UIElementProperties &properties)
    {
        m_Properties = properties;
        return *this;
    }

    UISpecification &Children(const std::initializer_list<ChildSpecification> &children);

    EntityId Build(Registry &registry)
    {
        return build_impl(registry, NULL_ENTITY);
    }

  public:
  private:
    EntityId build_impl(Registry &registry, EntityId parent);

    EntityId build_self(Registry &registry, EntityId parent);

  private:
    std::vector<ChildSpecification> m_Children;
    UIElementProperties m_Properties;
};
