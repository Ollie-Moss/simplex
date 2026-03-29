#include "core/Types.h"
#include <set>

class System
{
  public:
    System() {}
    System(const Signature &signature) : m_Signature(signature) {}

    virtual ~System() = default;

    virtual void OnCreate() {}
    virtual void Start() {}
    virtual void FixedUpdate(float timeStep) {}
    virtual void Update(float timeStep) {}

  public:
    std::set<EntityId> m_Entities;
    Signature m_Signature;

  protected:
    // Context
  private:
};
