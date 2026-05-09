#pragma once

#include "UIBuilderTypes.h"
#include "gui/Text.h"
#include "gui/UIComponents.h"
#include "systems/System.h"
#include "core/Registry.h"
#include <iostream>
#include "graphics/render-commands/ColliderCommand.h"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/render-commands/TextCommand.h"

class UIRenderSystem : public System
{
  public:
    UIRenderSystem(Registry &registry, const SimplexModules &modules) : System(registry, modules)
    {
        m_Signature = m_Registry.CreateSignature<UIElement, UITransform>();
    }

    void Update(float timeStep) override
    {
        for(EntityId e : m_Entities)
        {
            UIElement element = m_Registry.GetComponent<UIElement>(e);
            if(element.parent != NULL_ENTITY)
                continue;

            RenderElements(e);
        }
    }
    void RenderElements(EntityId entity)
    {
        if(entity == NULL_ENTITY)
        {
            std::cout << "NULL ENTITY SOMEHOW" << "\n";
        }
        UIElement &element = m_Registry.GetComponent<UIElement>(entity);
        UITransform &transform = m_Registry.GetComponent<UITransform>(entity);
        UILayout &layout = m_Registry.GetComponent<UILayout>(entity);
        UIStyle &style = m_Registry.GetComponent<UIStyle>(entity);
        Text &text = m_Registry.GetComponent<Text>(entity);
        TextLayout &textLayout = m_Registry.GetComponent<TextLayout>(entity);

        SpriteCommand cmd = {.sprite = {NO_TEXTURE, style.color.Get()}, .transform = transform, .renderSpace = RenderSpace::Screen};
        ColliderCommand debugCmd = {
            .transform = transform,
        };

        // Simplex::GetRendererManager().Submit<ColliderCommand>(debugCmd);

        m_Modules.m_RendererManager->Submit<SpriteCommand>(cmd);

        if(!text.content.Get().empty())
        {
            glm::vec2 pos = transform.position;

            TextCommand cmd = {.glyphs = textLayout.glyphs, .position = pos, .color = text.color, .clipRect = glm::vec4(transform.position, transform.size)};
            // Simplex::GetRendererManager().Submit<ColliderCommand>({.transform = {glm::vec2(pos, 0), textLayout.size}});
            m_Modules.m_RendererManager->Submit<TextCommand>(cmd);
        }

        for(EntityId child : element.children)
        {
            RenderElements(child);
        }
    }
};
