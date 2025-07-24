#pragma once

#include "core/ComponentManager.h"
#include "core/EntityManager.h"
#include "core/SystemManager.h"
#include <array>
#include <cstddef>
#include <vector>

class Registry {
  public:
    Registry() {};

    Entity Create() {
        Entity entity = m_EntityManager.CreateEntity();
        m_Entities[entityIndex] = entity;
        entityIndex++;
        return entity;
    }
    void Remove(Entity entity) {
        m_EntityManager.DestroyEntity(entity);
        m_Entities[entity] = m_Entities[entityIndex];
        entityIndex--;
    }

    template <typename T> void AddComponent(EntityId entity, T component) {
        m_ComponentManager.AddComponent(entity, component);
        ComponentId componentId = m_ComponentManager.GetComponentType<T>();

        Signature signature = m_EntityManager.GetSignature(entity);
        signature.set(componentId, true);
        m_EntityManager.SetSignature(entity, signature);
        m_SystemManager.EntitySignatureChanged(entity, signature);
    }

    template <typename T> void RemoveComponent(EntityId entity) {
        m_ComponentManager.RemoveComponent<T>(entity);

        ComponentId componentId = m_ComponentManager.GetComponentType<T>();

        Signature signature = m_EntityManager.GetSignature(entity);
        signature.set(componentId, false);
        m_EntityManager.SetSignature(entity, signature);
        m_SystemManager.EntitySignatureChanged(entity, signature);
    }

    template <typename T> T GetComponent(EntityId entity) {
        return m_ComponentManager.GetComponent<T>(entity);
    }

    template <typename T> void RegisterSystem(System system) {
        m_SystemManager.RegisterSystem<T>();
    }

    void Update() {
        m_SystemManager.UpdateSystems();
    }

    template <typename... T> Signature CreateSignature() {
        Signature signature;
        // Create Signature based on Components provided in template
        ([&] { signature.set(m_ComponentManager.GetComponentType<T>(), true); }(), ...);
        return signature;
    }

    template <typename... T> std::vector<Entity> View() {
        Signature viewSignature = CreateSignature<T...>();

        std::vector<Entity> entities;

        for (size_t i = 0; i < entityIndex; i++) {
            Entity entity = m_Entities[i];
            Signature entitySignature = m_EntityManager.GetSignature(entity);
            if (entitySignature == viewSignature) {
                entities.push_back(entity);
            }
        }
        return entities;
    }

  private:
    std::array<EntityId, MAX_ENTITIES> m_Entities;

    EntityManager m_EntityManager;
    ComponentManager m_ComponentManager;
    SystemManager m_SystemManager;

    size_t entityIndex = 0;
};
