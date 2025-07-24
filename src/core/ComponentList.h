#pragma once

#include "core/Types.h"
#include <unordered_map>


class IComponentList {
  public:
    virtual ~IComponentList() = default;
    virtual void EntityDestroyed(EntityId entity) = 0;
};

template <typename T> class ComponentList : public IComponentList {
  public:
    ComponentList() {}
    ~ComponentList() override {}

    void AddData(EntityId entity, T data) {
        m_ComponentData[index] = data;
        m_EntityToIndexMap[entity] = index;
        m_IndexToEntityMap[index] = entity;

        index++;
    }
    void RemoveData(EntityId entityToRemove) {
        size_t dataIndex = m_EntityToIndexMap[entityToRemove];

        m_ComponentData[dataIndex] = m_ComponentData[index - 1];

        EntityId entityToMove = m_IndexToEntityMap[index - 1];
        m_EntityToIndexMap[entityToMove] = dataIndex;
        m_IndexToEntityMap[dataIndex] = entityToMove;

        m_IndexToEntityMap.erase(index - 1);
        m_EntityToIndexMap.erase(entityToRemove);

        index--;
    }

    T &GetData(EntityId entity) {
        std::size_t dataIndex = m_EntityToIndexMap[entity];
        return m_ComponentData[dataIndex];
    }

    void EntityDestroyed(EntityId entity) override {
        if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end()) {
            // Remove the entity's component if it existed
            RemoveData(entity);
        }
    }

  private:
    std::array<T, MAX_COMPONENTS> m_ComponentData;
    std::unordered_map<EntityId, std::size_t> m_EntityToIndexMap;
    std::unordered_map<std::size_t, EntityId> m_IndexToEntityMap;

    // Current index of last data
    size_t index;
};
