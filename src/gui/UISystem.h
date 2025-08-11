#pragma once

#include "core/Simplex.h"
#include "core/SystemManager.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "glm/glm.hpp"
#include "graphics/Renderer2D.h"
#include "gui/UIComponents.h"
#include <cmath>
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
            auto [element, properties, transform] = e.GetComponents<UIElement, UIProperties, UITransform>();
            if(element.parent != NULL_ENTITY)
                continue;

            // this should only happen if the tree changes
            if(Simplex::GetView().HasWindowResized())
            {
                CalculateLayout(e);
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

    Axis &GetAxis(Sizing &size, FlexDirection direction)
    {
        if(direction == FlexDirection::Row)
        {
            return size.width;
        }
        return size.height;
    }
    float GetPadding(const Padding &padding, FlexDirection direction)
    {
        if(direction == FlexDirection::Row)
        {
            return padding.left + padding.right;
        }
        return padding.top + padding.bottom;
    }

    float &GetElementLength(Entity entity, FlexDirection direction)
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
            float length = GetElementLength(child, direction);
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
            total += GetElementLength(child, direction);
        }
        return total;
    }

    std::vector<EntityId> GetGrowableChildren(Entity entity, FlexDirection direction)
    {
        std::vector<EntityId> growables;
        for(Entity child : entity.GetComponent<UIElement>().children)
        {
            if(GetAxis(child.GetComponent<UIProperties>().sizing, direction).mode == SizingMode::Grow)
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
        if(GetAxis(properties.sizing, sizingAxis).mode != SizingMode::Hug)
            return;

        float gap = glm::max(0, (int)element.children.size() - 1) * properties.gap;
        float padding = GetPadding(properties.padding, sizingAxis);

        // Size with layout direction
        if(sizingAxis == properties.direction)
        {
            float &length = GetElementLength(entity, sizingAxis);
            length = SumChildrenLengths(entity, sizingAxis) + padding + gap;
        }
        else
        {
            // Size across layout direction
            float &length = GetElementLength(entity, sizingAxis);
            length = GetLargestChildLength(entity, sizingAxis) + padding + gap;
        }
    }

    void GrowSize(Entity entity, FlexDirection sizingAxis)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        if(element.parent == NULL_ENTITY)
        {
            float &length = GetElementLength(entity, sizingAxis);
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

        float remainingLength = GetElementLength(entity, sizingAxis);
        remainingLength -= GetPadding(properties.padding, sizingAxis);

        if(sizingAxis != properties.direction)
        {
            // Size across layout direction
            for(Entity child : growables)
            {
                float &length = GetElementLength(child, sizingAxis);

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
            remainingLength -= GetElementLength(child, sizingAxis);
        }

        const float EPSILON = 0.01f;
        int maxIterations = 100;
        while(remainingLength > EPSILON && maxIterations-- > 0)
        {
            float smallest = GetElementLength(growables[0], sizingAxis);
            float secondSmallest = INFINITY;
            float widthToAdd = remainingLength;

            for(Entity child : growables)
            {
                float childLength = GetElementLength(child, sizingAxis);
                if(childLength < smallest)
                {
                    secondSmallest = smallest;
                    smallest = childLength;
                }
                if(childLength > smallest)
                {
                    secondSmallest = glm::min(secondSmallest, childLength);
                    widthToAdd = secondSmallest - smallest;
                }
            }
            widthToAdd = glm::min(widthToAdd, remainingLength / growables.size());
            for(Entity child : growables)
            {
                float &length = GetElementLength(child, sizingAxis);
                if(length == smallest)
                {
                    length += widthToAdd;
                    remainingLength -= widthToAdd;
                }
            }
        }
        for(Entity child : element.children)
        {
            GrowSize(child, sizingAxis);
        }
    }

    void SizeFixedElements(Entity entity, FlexDirection sizingAxis)
    {
        // Applies the size length on elements with sizing mode fixed
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        float &length = GetElementLength(entity, sizingAxis);
        if(GetAxis(properties.sizing, sizingAxis).mode == SizingMode::Fixed)
        {
            length = GetAxis(properties.sizing, sizingAxis).length;
        }

        for(auto child : element.children)
        {
            SizeFixedElements(child, sizingAxis);
        }
    }

    void CalculatePositions(Entity entity, glm::vec2 parentPosition, FlexDirection direction)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UIProperties, UITransform>();

        float currentOffset = (direction == FlexDirection::Row) ? properties.padding.left : properties.padding.top;

        for(Entity child : element.children)
        {
            auto &childTransform = child.GetComponent<UITransform>();

            glm::vec2 localPos = {};
            if(direction == FlexDirection::Row)
            {
                localPos = {currentOffset, properties.padding.top};
                currentOffset += childTransform.size.x + properties.gap;
            }
            else
            {
                localPos = {properties.padding.left, currentOffset};
                currentOffset += childTransform.size.y + properties.gap;
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
