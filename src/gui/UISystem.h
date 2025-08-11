#pragma once

#include "core/Simplex.h"
#include "core/SystemManager.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "glm/common.hpp"
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
        for(Entity e : m_Entities)
        {
            auto [element, properties, transform] = e.GetComponents<UIElement, UIProperties, UITransform>();
            if(element.parent != NULL_ENTITY)
                continue;

            // this should only happen if the tree changes
            CalculateLayout(e);
            Simplex::GetRenderer().RenderImmediate(transform, NO_TEXTURE, properties.color);
            RenderElementsChildren(e, glm::vec2(0, 0));
        }
    }

    void CalculateLayout(Entity entity)
    {
        // Apply fixed sizes to transform
        SizeFixedElements(entity);

        // HUG Sizing along flex direction
        SizeHugElements(entity);

        // GROW Sizing along flex direction

        // Wrap Text

        // HUG Sizing across flex direction

        // GROW Sizing across flex direction

        // Calculate Positions
    }

    void SizeFixedElements(Entity entity)
    {
        // Applies the size length on elements with sizing mode fixed
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();
        if(properties.sizing.width.mode == SizingMode::Fixed)
        {
            transform.size.x = properties.sizing.width.length;
        };
        if(properties.sizing.height.mode == SizingMode::Fixed)
        {
            transform.size.y = properties.sizing.height.length;
        };
        for(auto child : element.children)
        {
            SizeFixedElements(child);
        }
    }
    glm::vec2 SizeHugElements(Entity entity)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();
        Entity parent = element.parent;
        auto parentProperties = parent.GetComponent<UIProperties>();

        for(auto child : element.children)
        {
            SizeHugElements(child);
        }

        glm::vec2 summedSize = glm::vec2(0, 0);
        for(auto child : element.children)
        {
            Entity childEntity = child;
            summedSize += childEntity.GetComponent<UITransform>().size;
        }

        // Apply children sizes to element
        switch(parentProperties.direction)
        {
        case FlexDirection::Row:
            if(properties.sizing.width.mode == SizingMode::Hug)
            {
                float gap = glm::max(0, (int)element.children.size() - 1) * properties.gap;
                float padding = properties.padding.left + properties.padding.right;
                transform.size.x = summedSize.x + padding + gap;
            }
            break;
        case FlexDirection::Column:
            if(properties.sizing.height.mode == SizingMode::Hug)
            {
                float gap = glm::max(0, (int)element.children.size() - 1) * properties.gap;
                float padding = properties.padding.top + properties.padding.bottom;
                transform.size.y = summedSize.y + padding + gap;
            }
            break;
        }
        return summedSize;
    };

    void RenderElementsChildren(Entity entity, glm::vec2 offset)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        float leftOffset = properties.padding.left;
        if(element.children.empty())
            return;

        for(Entity child : element.children)
        {
            auto [childElement, childProperties, childTransform] = child.GetComponents<UIElement, UIProperties, UITransform>();

            glm::vec2 childPosition = transform.position + childTransform.position + offset;
            childPosition.x += leftOffset;
            childPosition.y += properties.padding.top;

            Simplex::GetRenderer().RenderImmediate({.position = glm::vec3(childPosition, 0), .size = childTransform.size}, NO_TEXTURE, childProperties.color);
            leftOffset += childTransform.size.x + properties.gap;
            RenderElementsChildren(child, childPosition);
        }
    }

    void RecalculateLayout() {}
};
