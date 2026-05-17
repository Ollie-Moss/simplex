#pragma once

#include "core/ComponentManager.h"
#include "core/ComponentUpdaterManager.h"
#include "core/EntityManager.h"
#include "core/SimplexModules.h"
#include "core/SystemManager.h"
#include "core/Types.h"
#include <array>
#include <cstddef>
#include <memory>
#include <set>

class Registry
{
  public:
    Registry(const SimplexModules &modules) : m_SystemModules(modules) {};

    Registry() = default;
    ~Registry() = default;

    Registry(const Registry &) = default;
    Registry(Registry &&) = default;

    Registry &operator=(const Registry &) = default;
    Registry &operator=(Registry &&) = default;

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
        BuildEntity(entity, args...);
        return entity;
    }

    void Destroy(EntityId entity)
    {
        m_EntitiesToDelete.insert(entity);
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

    template <typename TComponent>
    ComponentId GetComponentId()
    {
        return m_ComponentManager.GetComponentType<TComponent>();
    }

    template <typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return m_SystemManager.RegisterSystem<T>(*this, m_SystemModules);
    }

    void AddUpdater(EntityId entity, std::shared_ptr<IComponentUpdater> updater)
    {
        m_ComponentUpdaters.AddUpdater(entity, updater);
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
        m_ComponentUpdaters.Update();

        DestroyEntities();

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
        // for(auto [id, entity] : m_EntitiesToCreate)
        // {
        //     entity.build();
        // }
        // m_EntitiesToCreate.clear();
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
            m_ComponentUpdaters.EntityDestroyed(entity);
        }
        m_EntitiesToDelete.clear();
    }

    template <typename... T>
    void BuildEntity(EntityId entity, T... args)
    {
        (AddComponent<T>(entity, args), ...);
    }

  private:
    SimplexModules m_SystemModules;

    std::array<EntityId, MAX_ENTITIES> m_Entities;
    std::set<EntityId> m_EntitiesToDelete;

    EntityManager m_EntityManager;
    ComponentManager m_ComponentManager;
    SystemManager m_SystemManager;

    ComponentUpdaterManager m_ComponentUpdaters;

    size_t entityIndex = 0;
};
