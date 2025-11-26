#pragma once

#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <vector>

enum class PhysicsType {
    Static,
    Dynamic
};

struct RigidBody2D
{
    PhysicsType type = PhysicsType::Dynamic;
    std::vector<glm::vec2> forces;
    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);

    void SetMass(float mass)
    {
        inverseMass = 1.0f / mass;
    }
    float inverseMass = 1.0f / 10.0f;
};
