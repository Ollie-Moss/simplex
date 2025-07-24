#pragma once

#include "core/ComponentManager.h"
#include "core/EntityManager.h"
#include "core/SystemManager.h"
#include <array>
#include <cstddef>
#include <memory>

class Registry {
  public:
    Registry() {};

    EntityId Create() {
        EntityId entity = m_EntityManager.CreateEntity();
        m_Entities[entityIndex] = entity;
        entityIndex++;
        return entity;
    }

    template <typename... T> EntityId Create(T... args) {
        EntityId entity = Create();
        ([&] { AddComponent<T>(entity, args); }(), ...);

        return entity;
    }

    void Remove(EntityId entity) {
        m_EntityManager.DestroyEntity(entity);
        m_Entities[entity] = m_Entities[entityIndex];
        entityIndex--;
    }

    template <typename T> void AddComponent(EntityId entity, T component) {
        m_ComponentManager.AddComponent<T>(entity, component);
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

    template <typename T> T &GetComponent(EntityId entity) {
        return m_ComponentManager.GetComponent<T>(entity);
    }

    template <typename T> std::shared_ptr<T> RegisterSystem() {
        return m_SystemManager.RegisterSystem<T>();
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

  private:
    std::array<EntityId, MAX_ENTITIES> m_Entities;

    EntityManager m_EntityManager;
    ComponentManager m_ComponentManager;
    SystemManager m_SystemManager;

    size_t entityIndex = 0;
};
