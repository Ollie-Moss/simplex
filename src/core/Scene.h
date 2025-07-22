#pragma once

#include "core/Component.h"
#include "core/Entity.h"
#include <array>
#include <cstdint>
#include <limits>

const std::uint32_t MAX_ENTITIES = std::numeric_limits<uint32_t>::max();
const std::uint32_t MAX_COMPONENTS = std::numeric_limits<uint32_t>::max();

class Scene {
  public:
    Scene();
    ~Scene();

  private:
  private:
    std::array<Entity, MAX_ENTITIES> m_Entities;
    std::array<Component, MAX_COMPONENTS> m_Components;
};
