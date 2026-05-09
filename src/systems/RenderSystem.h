#pragma once

#include "components/Sprite.h"
#include "components/Transform.h"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/util/RenderSpace.h"
#include "core/Registry.h"

class RenderSystem : public System
{
  public:
    RenderSystem(Registry &registry, const SimplexModules &modules) : System(registry, modules)
    {
        m_Signature = m_Registry.CreateSignature<Sprite, Transform>();
    }

    void Update(float timeStep) override
    {
        for(EntityId e : m_Entities)
        {
            Transform& transform = m_Registry.GetComponent<Transform>(e);
            Sprite& sprite = m_Registry.GetComponent<Sprite>(e);

            SpriteCommand cmd = {sprite, transform, RenderSpace::World};
            m_Modules.m_RendererManager->Submit<SpriteCommand>(cmd);
        }
    }
};
