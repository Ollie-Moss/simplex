#pragma once

#include "core/Types.h"
#include <gui/components/Text.h>
#include "glm/fwd.hpp"
#include <cctype>
#include <cmath>
#include <sys/types.h>
#include "core/Registry.h"
#include "gui/components/UIElement.h"
#include "gui/components/UITransform.h"

class UIEventSystem : public System
{
  public:
    UIEventSystem(Registry &registry, const SimplexModules &modules) : System(registry, modules)
    {
        m_Signature = m_Registry.CreateSignature<UIElement, UITransform, UILayout, UIStyle, Text, UIEvents>();
    }
    void Update(float timeStep) override
    {
        for(EntityId e : m_Entities)
        {
            UIElement &elem = m_Registry.GetComponent<UIElement>(e);
            UITransform &transform = m_Registry.GetComponent<UITransform>(e);
            UIEvents &events = m_Registry.GetComponent<UIEvents>(e);

            glm::vec2 mousePos = m_Modules.m_Input->GetMousePosition();
            const std::vector<int> &buttons = m_Modules.m_Input->GetMouseInput();

            for(auto &button : buttons)
            {
                // // OnClick
                // if(Intersecting(transform, mousePos))
                // {
                //     ClickEvent evt = {mousePos, button};
                //     if(events.onClick)
                //     {
                //         events.onClick(evt, e);
                //         elem.dirty = true;
                //     }
                // }
            }

            // Mouse Down

            // Mouse Up

            // Hover Enter

            // Hover Exit
        }
    }

    bool Intersecting(Transform transform, glm::vec2 position)
    {
        glm::vec2 min = transform.position;
        glm::vec2 max = glm::vec2(transform.position) + transform.size;

        return (position.x > min.x && position.y > min.y) && (position.x < max.x && position.y < max.y);
    }
};
