#pragma once

#include "core/SystemManager.h"
#include "components/Transform.h"
#include "core/Entity.h"
#include "core/SystemManager.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "graphics/render-commands/ColliderCommand.h"
#include "physics/RigidBody2D.h"
#include "physics/Collider2D.h"

class DebugPhysicsSystem : public System
{
  public:
    DebugPhysicsSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<Transform, Collider2D>();
    }

    void Update(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            auto [transform, collider] = e.GetComponents<Transform, Collider2D>();
            ColliderCommand cmd = {.transform = transform, .collider = collider};
            Simplex::GetRendererManager().Submit<ColliderCommand>(cmd);
        }
    }
};
