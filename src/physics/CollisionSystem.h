#pragma once

#include "core/SystemManager.h"
#include "components/Transform.h"
#include "core/Entity.h"
#include "core/SystemManager.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "physics/RigidBody2D.h"
#include "physics/Collider2D.h"
#include <memory>
#include <vector>

using NodeId = std::uint32_t;

static constexpr NodeId null = NodeId(-1);

class Quad
{
    glm::vec2 min;
    glm::vec2 max;

    Quad &operator|=(glm::vec2 const &p)
    {
        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);
        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
        return *this;
    }
};

struct Node
{
    NodeId children[2][2]{
        {null, null},
        {null, null}};
    Entity entity;
};

class QuadTree
{
    Quad quad;
    NodeId root;
    std::vector<Node> nodes;
};

template <typename Iterator>
QuadTree build(Iterator begin, Iterator end)
{
    QuadTree tree;

    return tree;
}

class CollisionSystem : public System
{
  public:
    QuadTree m_QuadTree;
    CollisionSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<Transform, RigidBody2D, Collider2D>();
    }

    void FixedUpdate(float timeStep) override
    {
    }

    std::vector<Entity> BroadPhase()
    {
        build(m_Entities.begin(), m_Entities.end());
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
