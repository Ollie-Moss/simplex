#include "core/Types.h"
#include <cassert>
#include <cstdint>
#include <memory>
#include "core/ComponentList.h"
#include <typeinfo>
#include <unordered_map>

class ComponentManager
{
  public:
    ComponentManager() {}

    template <typename T>
    void AddComponent(EntityId entity, T component)
    {
        if(!HasComponent<T>())
        {
            Register<T>();
        }
        GetComponentList<T>()->AddData(entity, component);
    };

    template <typename T>
    void RemoveComponent(EntityId entity)
    {
        GetComponentList<T>()->RemoveComponent(entity);
    };

    template <typename T>
    T &GetComponent(EntityId entity)
    {
        // Get a reference to a component from the array for an entity
        return GetComponentList<T>()->GetData(entity);
    }
    template <typename T>
    T *TryGetComponent(EntityId entity)
    {
        // Get a ptr to a component from the array for an entity if it exists
        return GetComponentList<T>()->TryGetData(entity);
    }

    template <typename T>
    bool HasComponent()
    {
        const char *typeName = typeid(T).name();
        return m_ComponentTypes.find(typeName) != m_ComponentTypes.end();
    }

    template <typename T>
    ComponentId GetComponentType()
    {
        if(!HasComponent<T>())
        {
            Register<T>();
        }
        const char *typeName = typeid(T).name();

        // Return this component's type - used for creating signatures
        return m_ComponentTypes[typeName];
    }

    void EntityDestroyed(EntityId entity)
    {
        // Notify each component array that an entity has been destroyed
        // If it has a component for that entity, it will remove it
        for(auto const &[id, component] : m_Components)
        {
            component->EntityDestroyed(entity);
        }
    }

    template <typename T>
    void Register()
    {
        const char *componentName = typeid(T).name();
        assert(m_ComponentTypes.find(componentName) == m_ComponentTypes.end() && "Registering component type more than once.");

        m_Components.insert({m_CurrentComponent, std::make_shared<ComponentList<T>>()});
        m_ComponentTypes.insert({componentName, m_CurrentComponent});

        m_CurrentComponent++;
    }

    int GetComponentCount()
    {
        int total = 0;
        for(auto [id, list] : m_Components)
        {
            total += list->GetComponentCount();
        }
        return total;
    }

  private:
    template <typename T>
    std::shared_ptr<ComponentList<T>> GetComponentList()
    {
        const char *componentName = typeid(T).name();
        std::uint32_t component = m_ComponentTypes[componentName];
        return std::static_pointer_cast<ComponentList<T>>(m_Components[component]);
    }

  private:
    std::unordered_map<ComponentId, std::shared_ptr<IComponentList>> m_Components;

    std::unordered_map<const char *, ComponentId> m_ComponentTypes{};

    ComponentId m_CurrentComponent = 0;
};
