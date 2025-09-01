#pragma once

#include "core/ComponentManager.h"
#include "core/EntityManager.h"
#include "core/SystemManager.h"
#include "core/Types.h"
#include <array>
#include <cstddef>
#include <memory>

class Registry
{
  public:
    Registry() {};
    ~Registry() = default;

    EntityId Create()
    {
        EntityId entity = m_EntityManager.CreateEntity();
        m_Entities[entityIndex] = entity;
        entityIndex++;
        return entity;
    }

    template <typename... T>
    EntityId Create(T... args)
    {
        EntityId entity = Create();
        ([&] { AddComponent<T>(entity, args); }(), ...);
        return entity;
    }

    void Remove(EntityId entity)
    {
        m_EntityManager.DestroyEntity(entity);
        m_Entities[entity] = m_Entities[entityIndex];
        entityIndex--;
    }

    template <typename T>
    void AddComponent(EntityId entity, T component)
    {
        m_ComponentManager.AddComponent<T>(entity, component);
        ComponentId componentId = m_ComponentManager.GetComponentType<T>();

        Signature signature = m_EntityManager.GetSignature(entity);
        signature.set(componentId, true);
        m_EntityManager.SetSignature(entity, signature);
        m_SystemManager.EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    void RemoveComponent(EntityId entity)
    {
        m_ComponentManager.RemoveComponent<T>(entity);

        ComponentId componentId = m_ComponentManager.GetComponentType<T>();

        Signature signature = m_EntityManager.GetSignature(entity);
        signature.set(componentId, false);
        m_EntityManager.SetSignature(entity, signature);
        m_SystemManager.EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    T &GetComponent(EntityId entity)
    {
        assert(entity <= MAX_ENTITIES && "CANNOT ACCESS NULL ENTITY COMPONENTS");
        return m_ComponentManager.GetComponent<T>(entity);
    }

    template <typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return m_SystemManager.RegisterSystem<T>();
    }

    void Start()
    {
        m_SystemManager.StartSystems();
    }
    void Update()
    {
        m_SystemManager.UpdateSystems();
    }
    void FixedUpdate()
    {
        m_SystemManager.FixedUpdateSystems();
    }

    template <typename... T>
    Signature CreateSignature()
    {
        Signature signature;
        // Create Signature based on Components provided in template
        ([&] { signature.set(m_ComponentManager.GetComponentType<T>(), true); }(), ...);
        return signature;
    }

    template <typename... T>
    std::vector<EntityId> View()
    {
        Signature viewSignature = CreateSignature<T...>();
        std::vector<EntityId> entities;

        for(size_t i = 0; i < entityIndex; i++)
        {
            EntityId entity = m_Entities[i];
            Signature entitySignature = m_EntityManager.GetSignature(entity);
            if(entitySignature == viewSignature)
            {
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
