#pragma once

#include "core/Simplex.h"
#include "core/SystemManager.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "graphics/Renderer2D.h"
#include "gui/UIComponents.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>

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

        // HUG Sizing width
        SizeHugWidths(entity);

        // GROW Sizing width
        std::vector<EntityId> widthGrowables;
        for(Entity e : entity.GetComponent<UIElement>().children)
        {
            if(e.GetComponent<UIProperties>().sizing.width.mode == SizingMode::Grow)
            {
                widthGrowables.push_back(e);
            }
        }
        SizeGrowWidths(entity, widthGrowables);

        // Wrap Text - do later

        // HUG Sizing across flex direction
        std::vector<EntityId> heightGrowables;
        for(Entity e : entity.GetComponent<UIElement>().children)
        {
            if(e.GetComponent<UIProperties>().sizing.height.mode == SizingMode::Grow)
            {
                heightGrowables.push_back(e);
            }
        }
        SizeHugHeights(entity);

        // GROW Sizing across flex direction
        SizeGrowHeights(entity, heightGrowables);

        // Calculate Positions
    }

    void SizeGrowWidths(Entity parent, std::vector<EntityId> growable)
    {
        auto [element, properties, transform] = parent.GetComponents<UIElement, UIProperties, UITransform>();

        float remainingWidth = transform.size.x;
        remainingWidth -= properties.padding.left + properties.padding.right;

        if(properties.direction == FlexDirection::Column)
        {
            for(Entity child : growable)
            {
                auto &childTransform = child.GetComponent<UITransform>();
                childTransform.size.x = remainingWidth;
            }
            for(Entity child : element.children)
            {
                std::vector<EntityId> growables;
                for(Entity e : child.GetComponent<UIElement>().children)
                {
                    if(e.GetComponent<UIProperties>().sizing.width.mode == SizingMode::Grow)
                    {
                        growables.push_back(e);
                    }
                }
                if(growables.size() <= 0)
                    return;
                SizeGrowWidths(child, growables);
            }
            return;
        }
        remainingWidth -= glm::max(0, (int)element.children.size() - 1) * properties.gap;

        for(Entity child : element.children)
        {
            remainingWidth -= child.GetComponent<UITransform>().size.x;
        }
        while(remainingWidth > 0)
        {
            float smallest = ((Entity)growable[0]).GetComponent<UITransform>().size.x;
            float secondSmallest = INFINITY;
            float widthToAdd = remainingWidth;

            for(Entity child : growable)
            {
                auto childTransform = child.GetComponent<UITransform>();
                if(childTransform.size.x < smallest)
                {
                    secondSmallest = smallest;
                    smallest = childTransform.size.x;
                }
                if(childTransform.size.x > smallest)
                {
                    secondSmallest = glm::min(secondSmallest, childTransform.size.x);
                    widthToAdd = secondSmallest - smallest;
                }
            }
            widthToAdd = glm::min(widthToAdd, remainingWidth / growable.size());
            for(Entity child : growable)
            {
                auto &childTransform = child.GetComponent<UITransform>();
                if(childTransform.size.x == smallest)
                {
                    childTransform.size.x += widthToAdd;
                    remainingWidth -= widthToAdd;
                }
            }
        }
        for(Entity child : element.children)
        {
            std::vector<EntityId> growables;
            for(Entity e : child.GetComponent<UIElement>().children)
            {
                if(e.GetComponent<UIProperties>().sizing.width.mode == SizingMode::Grow)
                {
                    growables.push_back(e);
                }
            }
            if(growables.size() <= 0)
                return;
            SizeGrowWidths(child, growables);
        }
    }

    void SizeGrowHeights(Entity parent, std::vector<EntityId> growable)
    {
        auto [element, properties, transform] = parent.GetComponents<UIElement, UIProperties, UITransform>();

        float remainingHeight = transform.size.y;
        remainingHeight -= properties.padding.top + properties.padding.bottom;

        if(properties.direction == FlexDirection::Row)
        {
            for(Entity child : growable)
            {
                auto &childTransform = child.GetComponent<UITransform>();
                childTransform.size.y = remainingHeight;
            }
            for(Entity child : element.children)
            {
                std::vector<EntityId> growables;
                for(Entity e : child.GetComponent<UIElement>().children)
                {
                    if(e.GetComponent<UIProperties>().sizing.height.mode == SizingMode::Grow)
                    {
                        growables.push_back(e);
                    }
                }
                if(growables.size() <= 0)
                    return;
                SizeGrowHeights(child, growables);
            }
            return;
        }
        remainingHeight -= glm::max(0, (int)element.children.size() - 1) * properties.gap;

        for(Entity child : element.children)
        {
            remainingHeight -= child.GetComponent<UITransform>().size.y;
        }
        while(remainingHeight > 0)
        {
            float smallest = ((Entity)growable[0]).GetComponent<UITransform>().size.y;
            float secondSmallest = INFINITY;
            float heightToAdd = remainingHeight;

            for(Entity child : growable)
            {
                auto childTransform = child.GetComponent<UITransform>();
                if(childTransform.size.y < smallest)
                {
                    secondSmallest = smallest;
                    smallest = childTransform.size.y;
                }
                if(childTransform.size.y > smallest)
                {
                    secondSmallest = glm::min(secondSmallest, childTransform.size.y);
                    heightToAdd = secondSmallest - smallest;
                }
            }
            heightToAdd = glm::min(heightToAdd, remainingHeight / growable.size());
            for(Entity child : growable)
            {
                auto &childTransform = child.GetComponent<UITransform>();
                if(childTransform.size.y == smallest)
                {
                    childTransform.size.y += heightToAdd;
                    remainingHeight -= heightToAdd;
                }
            }
        }
        for(Entity child : element.children)
        {
            std::vector<EntityId> growables;
            for(Entity e : child.GetComponent<UIElement>().children)
            {
                if(e.GetComponent<UIProperties>().sizing.height.mode == SizingMode::Grow)
                {
                    growables.push_back(e);
                }
            }
            if(growables.size() <= 0)
                return;
            SizeGrowHeights(child, growables);
        }
    }

    void SizeHugWidths(Entity entity)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        for(auto child : element.children)
        {
            SizeHugWidths(child);
        }
        if(properties.sizing.width.mode != SizingMode::Hug)
            return;

        float gap = glm::max(0, (int)element.children.size() - 1) * properties.gap;
        float padding = properties.padding.left + properties.padding.right;

        // ROW direction by default
        FlexDirection direction = (element.parent == NULL_ENTITY) ? FlexDirection::Row : ((Entity)element.parent).GetComponent<UIProperties>().direction;

        if(direction == FlexDirection::Column)
        {
            // set to tallest child
            float largest = 0.0f;
            for(Entity child : element.children)
            {
                float width = child.GetComponent<UITransform>().size.x;
                if(width > largest)
                {
                    largest = width;
                }
            }
            transform.size.x = largest + gap + padding;
            return;
        }

        glm::vec2 summedSize = glm::vec2(0, 0);
        for(auto child : element.children)
        {
            Entity childEntity = child;
            summedSize += childEntity.GetComponent<UITransform>().size;
        }

        // Apply children sizes to element
        transform.size.x = summedSize.x + padding + gap;
    };
    void SizeHugHeights(Entity entity)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        for(auto child : element.children)
        {
            SizeHugHeights(child);
        }
        if(properties.sizing.height.mode != SizingMode::Hug)
            return;

        float gap = glm::max(0, (int)element.children.size() - 1) * properties.gap;
        float padding = properties.padding.top + properties.padding.bottom;

        // ROW direction by default
        FlexDirection direction = (element.parent == NULL_ENTITY) ? FlexDirection::Row : ((Entity)element.parent).GetComponent<UIProperties>().direction;

        if(direction == FlexDirection::Row)
        {
            // set to tallest child
            float largest = 0.0f;
            for(Entity child : element.children)
            {
                float height = child.GetComponent<UITransform>().size.y;
                if(height > largest)
                {
                    largest = height;
                }
            }
            transform.size.y = largest + gap + padding;
            return;
        }

        glm::vec2 summedSize = glm::vec2(0, 0);
        for(auto child : element.children)
        {
            Entity childEntity = child;
            summedSize += childEntity.GetComponent<UITransform>().size;
        }

        transform.size.y = summedSize.y + padding + gap;
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

    void RenderElementsChildren(Entity entity, glm::vec2 offset)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        float leftOffset;
        if(properties.direction == FlexDirection::Row)
        {
            leftOffset = properties.padding.left;
        }
        else if(properties.direction == FlexDirection::Column)
        {
            leftOffset = properties.padding.top;
        }

        if(element.children.empty())
            return;

        for(Entity child : element.children)
        {
            auto [childElement, childProperties, childTransform] = child.GetComponents<UIElement, UIProperties, UITransform>();

            glm::vec2 childPosition = transform.position + childTransform.position + offset;

            if(properties.direction == FlexDirection::Row)
            {
                childPosition.x += leftOffset;
                childPosition.y += properties.padding.top;
            }
            else if(properties.direction == FlexDirection::Column)
            {
                childPosition.x += properties.padding.left;
                childPosition.y += leftOffset;
            }

            Simplex::GetRenderer().RenderImmediate({.position = glm::vec3(childPosition, 0), .size = childTransform.size}, NO_TEXTURE, childProperties.color);

            if(properties.direction == FlexDirection::Row)
            {
                leftOffset += childTransform.size.x + properties.gap;
            }
            else if(properties.direction == FlexDirection::Column)
            {
                leftOffset += childTransform.size.y + properties.gap;
            }
            RenderElementsChildren(child, childPosition);
        }
    }
};
