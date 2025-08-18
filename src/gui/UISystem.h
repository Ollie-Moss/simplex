#pragma once

#include "core/Simplex.h"
#include "core/SystemManager.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "graphics/Renderer2D.h"
#include "gui/UIBuilder.h"
#include "gui/UIComponents.h"
#include <algorithm>
#include <cmath>
#include <complex>
#include <string>

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
            for(Entity e : m_Entities)
            {
                auto [element, properties, transform] = e.GetComponents<UIElement, UIProperties, UITransform>();
                if(element.parent != NULL_ENTITY)
                    continue;

                // this should only happen if the tree changes
                if(Simplex::GetView().HasWindowResized())
                {
                    CalculateLayout(e);
                }
            }
            RenderElements(e);
        }
    }

    void CalculateLayout(Entity entity)
    {
        auto [props, transform] = entity.GetComponents<UIProperties, UITransform>();

        // Apply fixed sizes to transform
        // SizeFixedElements(entity, FlexDirection::Row);
        // SizeFixedElements(entity, FlexDirection::Column);

        // HUG Sizing width
        HugSize(entity, FlexDirection::Row);

        // GROW Sizing width
        GrowSize(entity, FlexDirection::Row);

        // Wrap Text - do later

        // HUG Sizing heights
        HugSize(entity, FlexDirection::Column);

        // GROW Sizing heights
        GrowSize(entity, FlexDirection::Column);

        // Calculate Positions
        CalculatePositions(entity, glm::vec2(0.0f), props.direction);
    }

    Axis &GetAxis(Entity entity, FlexDirection direction)
    {
        UIProperties &props = entity.GetComponent<UIProperties>();
        if(direction == FlexDirection::Row)
        {
            return props.sizing.width;
        }
        return props.sizing.height;
    }
    float GetPadding(const Padding &padding, FlexDirection direction)
    {
        if(direction == FlexDirection::Row)
        {
            return padding.left + padding.right;
        }
        return padding.top + padding.bottom;
    }

    float &GetLengthWithAxis(Entity entity, FlexDirection direction)
    {
        if(direction == FlexDirection::Row)
        {
            return entity.GetComponent<UITransform>().size.x;
        }
        return entity.GetComponent<UITransform>().size.y;
    }
    float &GetLengthAgainstAxis(Entity entity, FlexDirection direction)
    {
        if(direction == FlexDirection::Row)
        {
            return entity.GetComponent<UITransform>().size.x;
        }
        return entity.GetComponent<UITransform>().size.y;
    }

    float GetLargestChildLength(Entity entity, FlexDirection direction)
    {
        float largest = 0.0f;
        for(Entity child : entity.GetComponent<UIElement>().children)
        {
            float length = GetLengthWithAxis(child, direction);
            if(length > largest)
            {
                largest = length;
            }
        }
        return largest;
    }

    float SumChildrenLengths(Entity entity, FlexDirection direction)
    {
        float total = 0.0f;
        for(Entity child : entity.GetComponent<UIElement>().children)
        {
            total += GetLengthWithAxis(child, direction);
        }
        return total;
    }

    std::vector<EntityId> GetGrowableChildren(Entity entity, FlexDirection direction)
    {
        std::vector<EntityId> growables;
        for(Entity child : entity.GetComponent<UIElement>().children)
        {
            if(GetAxis(child, direction).mode == SizingMode::Grow)
            {
                growables.push_back(child);
            }
        }
        return growables;
    }

    // direction - size width or height axis
    void HugSize(Entity entity, FlexDirection sizingAxis)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();
        if(element.children.empty())
            return;

        for(auto child : element.children)
        {
            HugSize(child, sizingAxis);
        }
        if(GetAxis(entity, sizingAxis).mode != SizingMode::Hug)
            return;

        float gap = glm::max(0, (int)element.children.size() - 1) * properties.gap;
        float padding = GetPadding(properties.padding, sizingAxis);

        // Size with layout direction
        if(sizingAxis == properties.direction)
        {
            float &length = GetLengthWithAxis(entity, sizingAxis);
            length = SumChildrenLengths(entity, sizingAxis) + padding + gap;
        }
        else
        {
            // Size across layout direction
            float &length = GetLengthWithAxis(entity, sizingAxis);
            length = GetLargestChildLength(entity, sizingAxis) + padding + gap;
        }
    }

    void GrowSize(Entity entity, FlexDirection sizingAxis)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        if(element.parent == NULL_ENTITY && GetAxis(entity, sizingAxis).mode == SizingMode::Grow)
        {
            float &length = GetLengthWithAxis(entity, sizingAxis);
            length = (sizingAxis == FlexDirection::Column ? Simplex::GetView().GetWindowHeight() : Simplex::GetView().GetWindowWidth());
        }
        std::vector<EntityId> growables = GetGrowableChildren(entity, sizingAxis);
        if(growables.empty())
        {
            for(Entity child : element.children)
            {
                GrowSize(child, sizingAxis);
            }
            return;
        }

        float remainingLength = GetLengthWithAxis(entity, sizingAxis);
        remainingLength -= GetPadding(properties.padding, sizingAxis);

        if(sizingAxis != properties.direction)
        {
            // Size across layout direction
            for(Entity child : growables)
            {
                float &length = GetLengthWithAxis(child, sizingAxis);

                // std::cout << remainingLength << "\n";
                length = remainingLength;
            }
            for(Entity child : element.children)
            {
                GrowSize(child, sizingAxis);
            }
            return;
        }

        // Size with layout direction
        remainingLength -= glm::max(0, (int)element.children.size() - 1) * properties.gap;

        for(Entity child : element.children)
        {
            remainingLength -= GetLengthWithAxis(child, sizingAxis);
        }
        ResizeChildren(entity, growables, remainingLength, sizingAxis);

        for(Entity child : element.children)
        {
            GrowSize(child, sizingAxis);
        }
    }

    void ResizeChildren(Entity parent, std::vector<EntityId> growables, float remainingLength, FlexDirection sizingAxis)
    {
        const float EPSILON = 0.01f;
        int maxIterations = 100;

        while(std::abs(remainingLength) > EPSILON && maxIterations-- > 0)
        {
            // std::cout << maxIterations << "\n";
            //  Calculate total length of growables
            float totalLength = 0.0f;
            for(Entity child : growables)
            {
                totalLength += GetLengthWithAxis(child, sizingAxis);
            }

            // If totalLength is zero, distribute evenly to avoid division by zero
            if(totalLength == 0.0f)
            {
                totalLength = growables.size();
            }

            // Distribute remainingLength proportionally (or evenly if totalLength == 0)
            float distributed = 0.0f;
            for(Entity child : growables)
            {
                float &length = GetLengthWithAxis(child, sizingAxis);
                float maxLength = (GetAxis(child, sizingAxis).length / 100.0f) * GetLengthWithAxis(parent, sizingAxis);
                float maxDelta = maxLength - length;

                float ratio = (length > 0) ? (std::abs(length) / totalLength) : (1.0f / growables.size());
                float delta = ratio * remainingLength;

                delta = std::min(delta, maxDelta);

                // Apply delta to length
                length += delta;
                distributed += delta;
            }

            remainingLength -= distributed;

            // If no meaningful distribution happened, break early
            if(std::abs(distributed) < EPSILON)
                break;
        }
    }

    void SizeFixedElements(Entity entity, FlexDirection sizingAxis)
    {
        // Applies the size length on elements with sizing mode fixed
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        float &length = GetLengthWithAxis(entity, sizingAxis);
        if(GetAxis(entity, sizingAxis).mode == SizingMode::Fixed)
        {
            length = GetAxis(entity, sizingAxis).length;
        }

        for(auto child : element.children)
        {
            SizeFixedElements(child, sizingAxis);
        }
    }

    void CalculatePositions(Entity entity, glm::vec2 parentPosition, FlexDirection direction)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        float justifyContentOffset = (direction == FlexDirection::Row) ? properties.padding.left : properties.padding.top;
        float alignItemsOffset = (direction == FlexDirection::Column) ? properties.padding.left : properties.padding.top;

        // Apply justify content positions
        if(direction == properties.direction)
        {
            float remainingLength = GetLengthWithAxis(entity, direction);
            remainingLength -= GetPadding(properties.padding, direction);
            remainingLength -= glm::max(0, (int)element.children.size() - 1) * properties.gap;
            for(Entity child : element.children)
            {
                remainingLength -= GetLengthWithAxis(child, direction);
            }
            if(properties.justifyContent == JustifyContent::Center)
            {
                justifyContentOffset += remainingLength / 2.0f;
            }
            if(properties.justifyContent == JustifyContent::End)
            {
                justifyContentOffset += remainingLength;
            }
        }
        else
        {
            // Apply align items positions
            float length = GetLengthAgainstAxis(entity, direction);

            for(Entity child : element.children)
            {
                length -= GetLengthWithAxis(child, direction);
            }

            if(properties.alignItems == AlignItems::Center)
            {
                justifyContentOffset += length / 2.0f;
            }
            if(properties.alignItems == AlignItems::End)
            {
                justifyContentOffset += length;
            }
        }

        // Calculate offset based on children
        for(Entity child : element.children)
        {
            auto &childTransform = child.GetComponent<UITransform>();

            glm::vec2 localPos = parentPosition;
            if(properties.direction == FlexDirection::Row)
            {
                localPos += glm::vec2(justifyContentOffset, alignItemsOffset);
                justifyContentOffset += childTransform.size.x + properties.gap;
            }
            else
            {
                localPos += glm::vec2(alignItemsOffset, justifyContentOffset);
                justifyContentOffset += childTransform.size.y + properties.gap;
            }

            // Assign the child's position relative to parent
            childTransform.position = localPos;

            // Recursively calculate children's positions
            CalculatePositions(child, localPos, properties.direction);
        }
    }

    void RenderElements(Entity entity)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();
        Simplex::GetRenderer().QueueUIObject({.position = glm::vec3(transform.position, 0), .size = transform.size}, NO_TEXTURE, properties.color);
        for(Entity child : element.children)
        {
            RenderElements(child);
        }
    }
};
