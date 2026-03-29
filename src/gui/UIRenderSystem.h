#pragma once

#include "core/Simplex.h"
#include "graphics/render-commands/ColliderCommand.h"
#include "gui/Text.h"
#include "core/SystemManager.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/render-commands/TextCommand.h"
#include "graphics/util/RenderSpace.h"
#include "gui/UIBuilderTypes.h"
#include "gui/UIComponents.h"
#include <cctype>
#include <cmath>
#include <iostream>
#include <string>
#include <sys/types.h>

class UIRenderSystem : public System
{
  public:
    UIRenderSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<UIElement, UITransform>();
    }
    void Update(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            UIElement element = e.GetComponent<UIElement>();
            if(element.parent != NULL_ENTITY)
                continue;

            RenderElements(e);
        }
    }
    void RenderElements(Entity entity)
    {
        if(entity == NULL_ENTITY)
        {
            std::cout << "NULL ENTITY SOMEHOW" << "\n";
        }
        auto [element, transform, layout, style, text, textLayout] = entity.GetComponents<UIElement, UITransform, UILayout, UIStyle, Text, TextLayout>();

        SpriteCommand cmd = {.sprite = {NO_TEXTURE, style.color.Get()}, .transform = transform, .renderSpace = RenderSpace::Screen};
        ColliderCommand debugCmd = {
            .transform = transform,
        };

        // Simplex::GetRendererManager().Submit<ColliderCommand>(debugCmd);

        Simplex::GetRendererManager().Submit<SpriteCommand>(cmd);

        if(!text.content.Get().empty())
        {
            glm::vec2 pos = transform.position;

            TextCommand cmd = {.glyphs = textLayout.glyphs, .position = pos, .color = text.color, .clipRect = glm::vec4(transform.position, transform.size)};
            // Simplex::GetRendererManager().Submit<ColliderCommand>({.transform = {glm::vec2(pos, 0), textLayout.size}});
            Simplex::GetRendererManager().Submit<TextCommand>(cmd);
        }

        for(Entity child : element.children)
        {
            RenderElements(child);
        }
    }
};
