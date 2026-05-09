#pragma once

#include "core/Simplex.h"
#include "gui/Text.h"
#include "core/SystemManager.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "gui/UIComponents.h"
#include <cctype>
#include <cmath>
#include <sys/types.h>

class UIEventSystem : public System
{
  public:
    UIEventSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<UIElement, UITransform, UILayout, UIStyle, Text, UIEvents>();
    }
    void Update(float timeStep) override
    {
        for(EntityId e : m_Entities)
        {
            auto [elem, transform, events] = e.GetComponents<UIElement, UITransform, UIEvents>();

            glm::vec2 mousePos = Simplex::GetInput().GetMousePosition();
            const std::vector<int> &buttons = Simplex::GetInput().GetMouseInput();

            for(auto &button : buttons)
            {
                // OnClick
                if(Intersecting(transform, mousePos))
                {
                    ClickEvent evt = {mousePos, button};
                    if(events.onClick)
                    {
                        events.onClick(evt, e);
                        elem.dirty = true;
                    }
                }
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
