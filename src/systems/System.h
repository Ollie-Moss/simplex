#pragma once

#include "core/SimplexModules.h"
#include "core/Types.h"
#include <set>

class Registry;

class System
{
  public:
    System(Registry &registry, const SimplexModules &modules) : m_Registry(registry), m_Modules(modules) {}

    virtual void OnCreate() {}
    virtual void Start() {}
    virtual void FixedUpdate(float timeStep) {}
    virtual void Update(float timeStep) {}

  public:
    std::set<EntityId> m_Entities;
    Signature m_Signature;

  protected:
    const SimplexModules m_Modules;
    Registry &m_Registry;

  private:
};
