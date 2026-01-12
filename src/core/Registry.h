#pragma once

#include "core/ComponentManager.h"
#include "core/EntityManager.h"
#include "core/SystemManager.h"
#include "core/Types.h"
#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <set>

struct PendingEntity
{
    EntityId id;
    std::function<void()> build;
};

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
    EntityId QueueCreate(T... args)
    {
        EntityId entity = Create();
        std::function<void()> build = [&, entity, args...]() { BuildEntity<T...>(entity, args...); };
        PendingEntity pending = {entity, build};

        m_EntitiesToCreate.insert({entity, pending});
        return entity;
    }

    template <typename... T>
    EntityId Create(T... args)
    {
        EntityId entity = Create();
        BuildEntity(entity, args...);
        return entity;
    }

    void Destroy(EntityId entity)
    {
        m_EntitiesToDelete.insert(entity);
    }

    template <typename T>
    void QueueComponent(EntityId entity, T component)
    {
        auto &pending = m_EntitiesToCreate[entity];
        auto oldBuild = pending.build;
        auto build = [&, oldBuild, entity, component]() {oldBuild(); AddComponent(entity, component); };
        pending.build = build;
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
        assert(entity <= MAX_ENTITIES && "NULL ENTITY Provided to GetComponent()");
        return m_ComponentManager.GetComponent<T>(entity);
    }
    template <typename T>
    T *TryGetComponent(EntityId entity)
    {
        assert(entity <= MAX_ENTITIES && "NULL ENTITY Provided to TryGetComponent()");
        return m_ComponentManager.TryGetComponent<T>(entity);
    }

    template <typename TComponent>
    ComponentId GetComponentId()
    {
        return m_ComponentManager.GetComponentType<TComponent>();
    }

    template <typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return m_SystemManager.RegisterSystem<T>();
    }

    int GetEntityCount()
    {
        return entityIndex;
    }

    int GetComponentCount()
    {
        return m_ComponentManager.GetComponentCount();
    }

    void Start()
    {
        m_SystemManager.StartSystems();
    }
    void Update(float timeStep)
    {
        DestroyEntities();
        CreateEntites();

        m_SystemManager.UpdateSystems(timeStep);
    }
    void FixedUpdate(float timeStep)
    {
        m_SystemManager.FixedUpdateSystems(timeStep);
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
    void CreateEntites()
    {
        for(auto [id, entity] : m_EntitiesToCreate)
        {
            entity.build();
        }
        m_EntitiesToCreate.clear();
    }

    void DestroyEntities()
    {
        for(auto entity : m_EntitiesToDelete)
        {
            m_EntityManager.DestroyEntity(entity);
            m_Entities[entity] = m_Entities[entityIndex];
            entityIndex--;

            m_SystemManager.EntityDestroyed(entity);
            m_ComponentManager.EntityDestroyed(entity);
        }
        m_EntitiesToDelete.clear();
    }

    template <typename... T>
    void BuildEntity(EntityId entity, T... args)
    {
        (AddComponent<T>(entity, args), ...);
    }

  private:
    std::array<EntityId, MAX_ENTITIES> m_Entities;
    std::set<EntityId> m_EntitiesToDelete;
    std::unordered_map<EntityId, PendingEntity> m_EntitiesToCreate;

    EntityManager m_EntityManager;
    ComponentManager m_ComponentManager;
    SystemManager m_SystemManager;

    size_t entityIndex = 0;
};
