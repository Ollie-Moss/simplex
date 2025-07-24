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

    void EntityDestroyed(Entity entity) {
        // Erase a destroyed entity from all system lists
        // mEntities is a set so no check needed
        for (auto const &pair : m_Systems) {
            auto const &system = pair.second;

            system->m_Entities.erase(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature) {
        // Notify each system that an entity's signature changed
        for (auto const &[type, system] : m_Systems) {
            auto const &systemSignature = system->m_Signature;

            // Entity signature matches system signature - insert into set
            if ((entitySignature & systemSignature) == systemSignature) {
                system->m_Entities.insert(entity);
            }
            // Entity signature does not match system signature - erase from set
            else {
                system->m_Entities.erase(entity);
            }
        }
    }

  private:
    // Map from system type string pointer to a system pointer
    std::unordered_map<const char *, std::shared_ptr<System>> m_Systems{};
};
