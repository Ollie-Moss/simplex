#pragma once

#include <ostream>
#include "components/SpriteRenderer.h"
#include "components/Transform.h"
#include "core/Entity.h"
#include "core/SystemManager.h"

class RenderSystem : public System
{
   public:
    RenderSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<SpriteRenderer, Transform>();
    }

    void Update() override
    {
        for (Entity e : m_Entities) {
            SpriteRenderer s = e.GetComponent<SpriteRenderer>();
            Transform t = e.GetComponent<Transform>();

            Sprite sprite = {.texture = s.texture, .position = t.position, .size = t.size, .color = s.color};

            Simplex::GetRenderer().Queue(sprite);
        }
    }
};
