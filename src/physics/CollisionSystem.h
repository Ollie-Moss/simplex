#pragma once

#include "core/SystemManager.h"
#include "components/Transform.h"
#include "core/Entity.h"
#include "core/SystemManager.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "physics/RigidBody2D.h"
#include "physics/Collider2D.h"

class CollisionSystem : public System
{
  public:
    CollisionSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<Transform, RigidBody2D, Collider2D>();
    }

    void FixedUpdate(float timeStep) override
    {
        for(Entity e : m_Entities)
        {}
    }

    std::vector<Entity> BroadPhase()
    {
        return {};
    }
    std::vector<Entity> NarrowPhase()
    {
        return {};
    }

    void Resolution()
    {
    }
};
