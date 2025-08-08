#pragma once

#include "core/SystemManager.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "graphics/Renderer2D.h"
#include "gui/UIComponents.h"

class UISystem : public System
{
   public:
    UISystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<UIElement, UIProperties, UITransform>();
    }

    void Update() override
    {
        for (Entity e : m_Entities) {
            auto [element, properties, transform] = e.GetComponents<UIElement, UIProperties, UITransform>();

            // queue children
            RenderElementsChildren(e, glm::vec2(0, 0));
            // queue root
            Simplex::GetRenderer().Queue(ProjectionType::ScreenSpace, transform, NO_TEXTURE, properties.color);
        }
    }

    void RenderElementsChildren(Entity entity, glm::vec2 offset)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        float leftOffset = properties.padding.left;
        if (element.children.empty()) return;

        for (Entity child : element.children) {
            auto [childElement, childProperties, childTransform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

            glm::vec2 childPosition = transform.position + childTransform.position + offset;
            childPosition.x += leftOffset;
            childPosition.y += properties.padding.top;

            leftOffset += childTransform.size.x + properties.gap;
            RenderElementsChildren(child, childPosition);

            Simplex::GetRenderer().Queue(ProjectionType::ScreenSpace, childTransform, NO_TEXTURE, childProperties.color);
        }
    }

    void RecalculateLayout() {}
};
