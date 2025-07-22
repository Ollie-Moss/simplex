#pragma once

#include "core/Component.h"
#include "core/ComponentList.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <typeinfo>
#include <unordered_map>

class ComponentManager {
  public:
    template <typename T>
    void AddComponent(T component) {

    };

  private:
    template <typename T> void Register() {
        const char *componentName;
        if (HasComponent<T>(&componentName)) {
            std::cout << "Component Already Registered" << std::endl;
            return;
        }
        m_ComponentTypes.insert({componentName, m_CurrentComponent});
        m_Components.insert({m_CurrentComponent, std::make_shared<ComponentList<T>>});

        m_CurrentComponent++;
    }

    template <typename T> bool HasComponent(const char *component) {
        const char *componentName = typeid(T).name();

        if (m_ComponentTypes.find(componentName) != nullptr) {
            component = componentName;
            return true;
        }
        return false;
    }

  private:
    std::unordered_map<std::uint32_t, std::shared_ptr<IComponentList>> m_Components;

    std::unordered_map<const char *, std::uint32_t> m_ComponentTypes;

    std::uint32_t m_CurrentComponent = 0;
};
