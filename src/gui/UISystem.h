#pragma once

#include "core/Simplex.h"
#include "core/SystemManager.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/util/RenderSpace.h"
#include "gui/UIComponents.h"
#include <algorithm>
#include <cmath>

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
        CalculatePositions(entity, glm::vec2(0.0f));
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
    float GetParentPadding(Entity entity, FlexDirection direction)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();
        Entity parent = element.parent;
        if(parent == NULL_ENTITY)
        {
            return 0.0f;
        }
        auto [parentElement, parentProperties] = parent.GetComponents<UIElement, UIProperties>();
        return GetPadding(parentProperties.padding, direction);
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
        if(direction != FlexDirection::Row)
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

            // // If no meaningful distribution happened, break early
            // if(std::abs(distributed) < EPSILON)
            //     break;
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

    void CalculatePositions(Entity entity, glm::vec2 parentPosition)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        Entity parent = element.parent;

        float justifyContentOffset = (properties.direction == FlexDirection::Row) ? properties.padding.left : properties.padding.top;
        float alignItemsOffset = (properties.direction == FlexDirection::Column) ? properties.padding.left : properties.padding.top;

        // Apply justify content positions
        float remainingLength = GetLengthWithAxis(entity, properties.direction);

        // remainingLength -= GetParentPadding(properties.padding, direction);
        remainingLength -= GetPadding(properties.padding, properties.direction);
        remainingLength -= glm::max(0, (int)element.children.size() - 1) * properties.gap;
        for(Entity child : element.children)
        {
            remainingLength -= GetLengthWithAxis(child, properties.direction);
        }
        if(properties.justifyContent == JustifyContent::Center)
        {
            justifyContentOffset += remainingLength / 2.0f;
        }
        if(properties.justifyContent == JustifyContent::End)
        {
            justifyContentOffset += remainingLength;
        }

        // Apply align items positions
        float length = GetLengthAgainstAxis(entity, properties.direction);

        // Against axis padding (invert the direction to get the correct padding)
        length -= GetPadding(properties.padding, (properties.direction == FlexDirection::Row ? FlexDirection::Column : FlexDirection::Row));

        float largestLength = 0;
        for(Entity child : element.children)
        {
            float length = GetLengthAgainstAxis(child, properties.direction);
            if(length > largestLength)
            {
                largestLength = length;
            }
        }
        length -= largestLength;

        if(properties.alignItems == AlignItems::Center)
        {
            alignItemsOffset += length / 2.0f;
        }
        if(properties.alignItems == AlignItems::End)
        {
            alignItemsOffset += length;
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
            CalculatePositions(child, localPos);
        }
    }

    void RenderElements(Entity entity)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        SpriteCommand cmd = {.sprite = {NO_TEXTURE, properties.color}, .transform = transform, .renderSpace = RenderSpace::Screen};
        Simplex::GetRendererManager().Submit<SpriteCommand>(&cmd);
        for(Entity child : element.children)
        {
            RenderElements(child);
        }
    }
};
