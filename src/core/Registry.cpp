#include "Registry.h"
#include "core/Entity.h"

Entity Registry::Create() {
    Entity entity = m_EntityManager.CreateEntity();
    m_Entities[entityIndex] = entity;
    entityIndex++;
    return entity;
}
void Registry::Remove(Entity entity) {
    m_EntityManager.DestroyEntity(entity);
    m_Entities[entity] = m_Entities[entityIndex];
    entityIndex--;
}
