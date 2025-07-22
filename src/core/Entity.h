#pragma once

#include <cstdint>

using EntityId = std::uint32_t;

class Entity {
  public:

    operator std::uint32_t() {
        return m_Id;
    }

  private:
    EntityId m_Id;
};
