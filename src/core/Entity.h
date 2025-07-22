#pragma once

#include <cstdint>

class Entity {
  public:

    operator std::uint32_t() {
        return m_Id;
    }

  private:
    std::uint32_t m_Id;
};
