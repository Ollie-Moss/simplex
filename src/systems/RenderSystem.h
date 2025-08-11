#pragma once

#include "components/Sprite.h"
#include "components/Transform.h"
#include "core/Entity.h"
#include "core/SystemManager.h"
#include "graphics/Renderer2D.h"

class RenderSystem : public System
{
  public:
    RenderSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<Sprite, Transform>();
    }

    void Update() override
    {
        for(Entity e : m_Entities)
        {
            auto [transform, sprite] = e.GetComponents<Transform, Sprite>();

            Simplex::GetRenderer().QueueWorldObject(transform, sprite.texture, sprite.color);
        }
    }
};
