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

class QT_Quad
{
  private:
    int threshold = 5;
    glm::vec2 minBound = glm::vec2(0, 0);
    glm::vec2 maxBound = glm::vec2(0, 0);

    std::vector<Entity> entities;

    std::shared_ptr<QT_Quad> botLeftTree;
    std::shared_ptr<QT_Quad> botRightTree;
    std::shared_ptr<QT_Quad> topLeftTree;
    std::shared_ptr<QT_Quad> topRightTree;

    bool inBoundary(glm::vec2 p)
    {
        return (p.x >= minBound.x && p.x <= maxBound.x && p.y >= minBound.y && p.y <= maxBound.y);
    }

    void insert(Entity node)
    {
        if(node == NULL_ENTITY)
            return;

        auto [transform] = node.GetComponents<Transform>();

        // Current quad cannot contain it
        if(!inBoundary(transform.position))
            return;

        // We are at a quad of unit area
        // We cannot subdivide this quad further
        if(std::abs(maxBound.x - minBound.x) <= 1 && std::abs(maxBound.y - minBound.y) <= 1)
        {
            return;
        }

        if((maxBound.x + minBound.x) / 2 >= transform.position.x)
        {
            // Indicates topLeftTree
            if((maxBound.y + minBound.y) / 2 >= transform.position.y)
            {
                if(topLeftTree == nullptr)
                {
                    glm::vec2 min = glm::vec2(maxBound.x, maxBound.y);
                    glm::vec2 max = glm::vec2((maxBound.x + minBound.x) / 2, (maxBound.y + minBound.y) / 2);

                    topLeftTree = std::make_shared<QT_Quad>(min, max);
                }

                topLeftTree->insert(node);
            }

            // Indicates botLeftTree
            else
            {
                if(botLeftTree == nullptr)
                {
                    glm::vec2 min = glm::vec2(maxBound.x, (maxBound.y + minBound.y) / 2);
                    glm::vec2 max = glm::vec2((maxBound.x + minBound.x) / 2, minBound.y);
                    botLeftTree = std::make_shared<QT_Quad>(min, max);
                }
                botLeftTree->insert(node);
            }
        }
        else
        {
            // Indicates topRightTree
            if((maxBound.y + minBound.y) / 2 >= transform.position.y)
            {
                if(topRightTree == nullptr)
                {
                    glm::vec2 min = glm::vec2((maxBound.x + minBound.x) / 2, maxBound.y);
                    glm::vec2 max = glm::vec2(minBound.x, (maxBound.y + minBound.y) / 2);
                    topRightTree = std::make_shared<QT_Quad>(min, max);
                }
                topRightTree->insert(node);
            }

            // Indicates botRightTree
            else
            {
                if(botRightTree == nullptr)
                {
                    glm::vec2 min = glm::vec2((maxBound.x + minBound.x) / 2, (maxBound.y + minBound.y) / 2);
                    glm::vec2 max = glm::vec2(minBound.x, minBound.y);
                    botRightTree = std::make_shared<QT_Quad>(min, max);
                }
                botRightTree->insert(node);
            }
        }
    }
};

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
