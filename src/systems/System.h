#include "core/Types.h"
#include <set>

class System {
  public:
    System() {}
    virtual ~System() = default;

    std::set<EntityId> m_Entities;
    // This should be set in the constructor
    Signature m_Signature;

    virtual void Start() {}
    virtual void Update() {}
};
