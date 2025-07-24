#pragma once

#include "systems/System.h"
#include <cassert>
#include <memory>
#include <typeinfo>
#include <unordered_map>

class SystemManager {
  public:
    SystemManager() {}
    template <typename T> std::shared_ptr<T> RegisterSystem() {
        const char *typeName = typeid(T).name();

        assert(m_Systems.find(typeName) == m_Systems.end() && "Registering system more than once.");

        // Create a pointer to the system and return it so it can be used externally
        auto system = std::make_shared<T>();
        m_Systems.insert({typeName, system});
        return system;
    }

    void UpdateSystems() {
        for (auto const &[name, system] : m_Systems) {
            system->Update();
        }
    }

    void EntityDestroyed(EntityId entity);

    void EntitySignatureChanged(EntityId entity, Signature entitySignature);

  private:
    // Map from system type string pointer to a system pointer
    std::unordered_map<const char *, std::shared_ptr<System>> m_Systems{};
};
