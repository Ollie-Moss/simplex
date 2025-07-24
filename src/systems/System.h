#include "core/Types.h"
#include <set>

class Entity;

class System {
  public:
    System() {}
    virtual ~System() = default;

    std::set<Entity> m_Entities;
    // This should be set in the constructor
    Signature m_Signature;

    virtual void Start() {}
    virtual void Update() {}
};
