#include "SystemManager.h"

void SystemManager::EntityDestroyed(EntityId entity) {
    // Erase a destroyed entity from all system lists
    // mEntities is a set so no check needed
    for (auto const &pair : m_Systems) {
        auto const &system = pair.second;

        system->m_Entities.erase(entity);
    }
}

void SystemManager::EntitySignatureChanged(EntityId entity, Signature entitySignature) {
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
