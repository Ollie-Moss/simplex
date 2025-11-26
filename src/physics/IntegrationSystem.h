#pragma once

#include "core/SystemManager.h"
#include "components/Transform.h"
#include "core/Entity.h"
#include "core/SystemManager.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "physics/RigidBody2D.h"

class IntegrationSystem : public System
{
  public:
    IntegrationSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<Transform, RigidBody2D>();
    }

    void FixedUpdate(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            auto [transform, rb] = e.GetComponents<Transform, RigidBody2D>();

            glm::vec2 forceSum = glm::vec2(0.0f, 0.0f);
            for(glm::vec2 force : rb.forces)
            {
                forceSum += force;
            }
            rb.forces.clear();

            // acceleration = force / mass (f = ma)
            auto acceleration = (forceSum * rb.inverseMass);
            acceleration += GRAVITY;

            // velocityDelta = acceleration * deltaTime (v = v₀ + at)
            rb.velocity += acceleration * timeStep;

            // positionDelta = velocity * deltaTime (v = Δd/Δt)
            glm::vec2 deltaPosition = rb.velocity * timeStep;
            transform.position += glm::vec3(deltaPosition, 0.0f);
        }
    }
};
