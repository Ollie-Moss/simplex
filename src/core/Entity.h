#pragma once

#include "core/Simplex.h"
#include <cassert>
#include <cstdint>
#include "core/Types.h"

class Entity {
  public:
    Entity();
    Entity(EntityId id) : m_Id(id) {}

    template <typename T> T& GetComponent() {
        return Simplex::GetRegistry().GetComponent<T>(m_Id);
    };
    template <typename T> void RemoveComponent() {
        // assert(m_Scene != nullptr);
        Simplex::GetRegistry().RemoveComponent<T>(m_Id);
    };

    operator std::uint32_t() {
        return m_Id;
    }

    bool operator<(const Entity &other) const {
        return m_Id < other.m_Id;
    }

  private:
    EntityId m_Id;
};
