#pragma once

#include "core/SystemManager.h"
#include "components/Transform.h"
#include "core/Entity.h"
#include "core/SystemManager.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "physics/RigidBody2D.h"
#include "physics/Collider2D.h"
#include "util/Quad.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>


constexpr uint32_t MAX_COLLISION_GROUP_SIZE = 5;
using CollisionGroup = std::array<EntityId, MAX_COLLISION_GROUP_SIZE>;

using NodeId = uint32_t;
constexpr NodeId NULL_NODE = -1;
constexpr size_t MAX_QUAD_TREE_SIZE = 10000;

class Node
{
  public:
    Node() {};
    NodeId children[4] = {
        NULL_NODE,
        NULL_NODE,
        NULL_NODE,
        NULL_NODE,
    };
    Quad bounds;
    glm::vec2 point;
    EntityId entity;

    void Split(glm::vec2 point)
    {
        glm::vec2 halfExtents = (bounds.max - bounds.min) / 2.0f;

        glm::vec2 center = bounds.max - halfExtents;

        Quad tl = Quad({bounds.min.x, center.y}, {center.x, bounds.max.y});
        Quad tr = Quad(center, bounds.max);
        Quad bl = Quad(bounds.min, center);
        Quad br = Quad({center.x, bounds.min.y}, {bounds.max.x, center.y});
    }
};

class QuadTree
{
  public:
    QuadTree() {}

  public:
    void Insert(EntityId entity, Transform transform, RigidBody2D rigidBody)
    {
    }

    void Remove(EntityId entity)
    {
        remove_impl(entity);
    }

    bool Contains(EntityId entity)
    {
        return m_Entities.contains(entity);
    }

    std::vector<CollisionGroup> CalculateCollisionGroups()
    {
        return {};
    }

  private:
    void insert_impl(EntityId entity, Transform transform)
    {
        for(NodeId nodeId : m_Root.children)
        {
            auto node = m_Tree[nodeId];
            if(node.bounds.InBounds(transform.position))
            {
            }
        }
    }

    void traverse(EntityId entity, NodeId nodeId, glm::vec2 point)
    {
        bool inBounds = m_Tree[nodeId].bounds.InBounds(point);
        if(!inBounds)
            return;

        bool hasChildren = false;
        for(NodeId childNodeId : m_Tree[nodeId].children)
        {
            if(childNodeId != NULL_NODE)
                hasChildren = true;
        }

        if(hasChildren)
        {
            for(NodeId childNodeId : m_Tree[nodeId].children)
            {
                if(childNodeId == NULL_NODE)
                    continue;

                traverse(entity, childNodeId, point);
            }
        }

        // Split this quad
    }

    void remove_impl(EntityId entity)
    {
    }

  private:
    std::vector<Quad> m_Quads;
    // Where NodeId corresponds to the index of a given node
    // This needs to be managed with respect to removing and inserting nodes
    std::array<Node, MAX_QUAD_TREE_SIZE> m_Tree;
    Node m_Root;
    size_t m_TreeIndex;

    // Stores contained entities in the quad tree for quick checking if a given entitiy is in the tree
    std::set<EntityId> m_Entities;
};

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
