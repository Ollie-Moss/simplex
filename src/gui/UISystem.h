#pragma once

#include "core/Simplex.h"
#include "core/SystemManager.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/render-commands/TextCommand.h"
#include "graphics/util/RenderSpace.h"
#include "gui/UIBuilder.h"
#include "gui/UIComponents.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <iomanip>
#include <string>
#include <sys/types.h>
#include <tuple>
#include <variant>
#include <vector>

class UIStateSystem : public System
{
  public:
    UIStateSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<UIElement, UITransform, UILayout, Text>();
    }
    void Update(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            auto [elem, trans, layout, text] = e.GetComponents<UIElement, UITransform, UILayout, Text>();

            bool anyChanged = false;
            std::apply(
                [&](auto &...b) {
                    ((anyChanged |= b.UpdateBinding()), ...);
                },
                layout.bindables());

            std::apply(
                [&](auto &...b) {
                    ((anyChanged |= b.UpdateBinding()), ...);
                },
                text.bindables());

            elem.dirty = anyChanged;
        }
    }
};

class UILayoutSystem : public System
{
  public:
    UILayoutSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<UIElement, UITransform, UILayout, Text>();
    }
    void Update(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            UIElement element = e.GetComponent<UIElement>();
            if(element.parent != NULL_ENTITY)
                continue;

            if(IsDirty(e))
            {
                CalculateLayout(e);
            }
        }
    }

    bool IsDirty(Entity entity)
    {
        UIElement element = entity.GetComponent<UIElement>();
        if(element.dirty || Simplex::GetView().HasWindowResized())
        {
            return true;
        }

        for(Entity e : element.children)
        {
            if(IsDirty(e))
            {
                return true;
            }
        }
        return false;
    }

    void CalculateLayout(Entity entity)
    {
        auto [elem, transform] = entity.GetComponents<UIElement, UITransform>();

        InitialSizing(entity, Direction::Horizontal);
        InitialSizing(entity, Direction::Vertical);

        // HUG Sizing width
        HugSize(entity, Direction::Horizontal);

        // GROW Sizing width
        GrowSize(entity, Direction::Horizontal);

        // Wrap Text
        WrapText(entity);

        // HUG Sizing heights
        HugSize(entity, Direction::Vertical);

        // GROW Sizing heights
        GrowSize(entity, Direction::Vertical);

        // Calculate Positions
        CalculatePositions(entity, glm::vec2(0.0f));

        // Marks all elements NOT dirty
        CleanTree(entity);
    }

    void CleanTree(Entity entity)
    {
        UIElement &elem = entity.GetComponent<UIElement>();
        elem.dirty = false;

        for(Entity e : elem.children)
        {
            CleanTree(e);
        }
    }

    Axis &GetAxis(Entity entity, Direction direction)
    {
        UILayout &props = entity.GetComponent<UILayout>();
        if(direction == Direction::Horizontal)
        {
            return props.sizing.Get().width.Get();
        }
        return props.sizing.Get().height.Get();
    }
    float GetPadding(const Padding &padding, Direction direction)
    {
        if(direction == Direction::Horizontal)
        {
            return padding.left + padding.right;
        }
        return padding.top + padding.bottom;
    }
    float GetParentPadding(Entity entity, Direction direction)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UILayout, UITransform>();
        Entity parent = element.parent;
        if(parent == NULL_ENTITY)
        {
            return 0.0f;
        }
        auto [parentElement, parentProperties] = parent.GetComponents<UIElement, UILayout>();
        return GetPadding(*parentProperties.padding, direction);
    }

    float &GetLengthWithAxis(Entity entity, Direction direction)
    {
        if(direction == Direction::Horizontal)
        {
            return entity.GetComponent<UITransform>().size.x;
        }
        return entity.GetComponent<UITransform>().size.y;
    }
    float &GetLengthAgainstAxis(Entity entity, Direction direction)
    {
        if(direction != Direction::Horizontal)
        {
            return entity.GetComponent<UITransform>().size.x;
        }
        return entity.GetComponent<UITransform>().size.y;
    }

    float GetLargestChildLength(Entity entity, Direction direction)
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

    float SumChildrenLengths(Entity entity, Direction direction)
    {
        float total = 0.0f;
        for(Entity child : entity.GetComponent<UIElement>().children)
        {
            total += GetLengthWithAxis(child, direction);
        }
        return total;
    }

    std::vector<EntityId> GetGrowableChildren(Entity entity, Direction direction)
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

    float MeasureText(Text text, Direction direction)
    {
        Font font = Simplex::GetAssetManager().Get<Font>(text.fontName);

        std::string::const_iterator c;

        float width = 0.0f;
        float height = 0.0f;
        float currentWidth = 0.0f;
        float largestWidth = 0.0f;
        float largestHeight = 0.0f;

        uint32_t lineCounter = 1;

        for(c = text.content.Get().begin(); c != text.content.Get().end(); c++)
        {
            Character ch = font.characters[*c];

            if(*c == '\n')
            {
                lineCounter += 1;
                largestWidth = std::max(currentWidth, largestWidth);
                currentWidth = 0;
            }
            else
            {
                currentWidth += (ch.Advance >> 6);
            }

            if(ch.Size.y > largestHeight)
            {
                largestHeight = ch.Size.y;
            }
        }

        width = std::max(currentWidth, largestWidth);

        float lineGaps = std::max<int>(0, lineCounter - 1) * text.lineHeight;
        float lineHeights = (lineCounter * largestHeight);
        height = lineHeights + lineGaps;

        return (direction == Direction::Horizontal) ? width : height;
    }

    void WrapText(Entity entity)
    {
        auto [element, properties, transform, text] = entity.GetComponents<UIElement, UILayout, UITransform, Text>();

        for(auto child : element.children)
        {
            WrapText(child);
        }

        if(text.content.Get().empty())
            return;

        float width = GetLengthWithAxis(entity, Direction::Horizontal);
        float &height = GetLengthWithAxis(entity, Direction::Vertical);
        Font font = Simplex::GetAssetManager().Get<Font>(text.fontName);

        float currentWidth = 0.0f;
        int widthCount = 0;
        float textWidth = 0.0f;
        float largestHeight = 0.0f;
        int lineCounter = 1;
        std::vector<int> breaks;

        for(int i = 0; i < text.content.Get().length(); i++)
        {
            char c = text.content.Get()[i];

            Character ch = font.characters[c];

            if(c == '\n')
            {
                currentWidth = 0;
                widthCount = 0;
                lineCounter++;
                continue;
            }
            else
            {
                currentWidth += (ch.Advance >> 6);
                widthCount++;
            }

            if(currentWidth + GetPadding(*properties.padding, Direction::Horizontal) > width && widthCount > 2)
            {
                breaks.push_back(i);
                currentWidth = (ch.Advance >> 6);
                widthCount = 0;
            }
            if(ch.Size.y > largestHeight)
            {
                largestHeight = ch.Size.y;
            }
        }
        text.breaks = breaks;

        int totalLines = breaks.size() + lineCounter;

        float lineGaps = std::max<int>(0, totalLines - 1) * text.lineHeight;
        float lineHeights = totalLines * largestHeight;
        height = lineHeights + lineGaps + GetPadding(*properties.padding, Direction::Horizontal);
    }

    void InitialSizing(Entity entity, Direction sizingAxis)
    {
        auto [element, properties, transform, text] = entity.GetComponents<UIElement, UILayout, UITransform, Text>();

        float &length = GetLengthWithAxis(entity, sizingAxis);
        Axis axis = GetAxis(entity, sizingAxis);

        float finalLength = 0.0f;

        auto padding = GetPadding(*properties.padding, sizingAxis);

        if(axis.length.unit == Unit::Percent)
        {
            float parentLength = sizingAxis == Direction::Vertical ? Simplex::GetView().GetWindowHeight() : Simplex::GetView().GetWindowWidth();

            if(element.parent != NULL_ENTITY)
            {
                parentLength = GetLengthWithAxis(element.parent, sizingAxis);
            }

            length = parentLength * axis.length.GetValue() + padding;
        }

        if(axis.length.unit == Unit::Pixels)
        {
            length = GetAxis(entity, sizingAxis).length.GetValue() + padding;
        }

        if(!text.content.Get().empty() && sizingAxis == Direction::Horizontal && axis.mode != SizingMode::Fixed)
        {
            auto textLength = MeasureText(text, Direction::Horizontal);
            if(length < textLength)
            {
                length = textLength;
            }
        }

        for(auto child : element.children)
        {
            InitialSizing(child, sizingAxis);
        }
    }

    // direction - size width or height axis
    void HugSize(Entity entity, Direction sizingAxis)
    {
        auto [element, properties, transform, text] = entity.GetComponents<UIElement, UILayout, UITransform, Text>();

        for(auto child : element.children)
        {
            HugSize(child, sizingAxis);
        }

        if(GetAxis(entity, sizingAxis).mode != SizingMode::Hug)
            return;

        float gap = glm::max(0, (int)element.children.size() - 1) * *properties.gap;
        float padding = GetPadding(properties.padding.Get(), sizingAxis);
        float textSize = MeasureText(text, sizingAxis);

        // Size with layout direction
        if(sizingAxis == properties.direction.Get())
        {
            float &length = GetLengthWithAxis(entity, sizingAxis);

            length = SumChildrenLengths(entity, sizingAxis) + padding + gap + textSize;
        }
        else
        {
            // Size across layout direction
            float &length = GetLengthWithAxis(entity, sizingAxis);
            length = GetLargestChildLength(entity, sizingAxis) + padding + gap + textSize;
        }
    }

    void GrowSize(Entity entity, Direction sizingAxis)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UILayout, UITransform>();

        if(element.parent == NULL_ENTITY && GetAxis(entity, sizingAxis).mode == SizingMode::Grow)
        {
            float &length = GetLengthWithAxis(entity, sizingAxis);
            length = (sizingAxis == Direction::Vertical ? Simplex::GetView().GetWindowHeight() : Simplex::GetView().GetWindowWidth());
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
        remainingLength -= GetPadding(*properties.padding, sizingAxis);

        if(sizingAxis != properties.direction)
        {
            // Size across layout direction
            for(Entity child : growables)
            {
                float &length = GetLengthWithAxis(child, sizingAxis);

                length = remainingLength;
            }
            for(Entity child : element.children)
            {
                GrowSize(child, sizingAxis);
            }
            return;
        }

        // Size with layout direction
        remainingLength -= glm::max(0, (int)element.children.size() - 1) * *properties.gap;

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

    void ResizeChildren(Entity parent, std::vector<EntityId> growables, float remainingLength, Direction sizingAxis)
    {
        const float EPSILON = 0.01f;
        int maxIterations = 100;

        while(std::abs(remainingLength) > EPSILON && maxIterations-- > 0)
        {
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
                float maxLength = GetAxis(child, sizingAxis).length.GetValue() * GetLengthWithAxis(parent, sizingAxis);
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

    void CalculatePositions(Entity entity, glm::vec2 parentPosition)
    {
        auto [element, properties, transform] = entity.GetComponents<UIElement, UILayout, UITransform>();

        Entity parent = element.parent;

        float justifyContentOffset = (properties.direction == Direction::Horizontal) ? properties.padding.Get().left : properties.padding.Get().top;
        float alignItemsOffset = (properties.direction == Direction::Vertical) ? properties.padding.Get().left : properties.padding.Get().top;

        // Apply justify content positions
        float remainingLength = GetLengthWithAxis(entity, *properties.direction);

        // remainingLength -= GetParentPadding(properties.padding, direction);
        remainingLength -= GetPadding(*properties.padding, *properties.direction);
        remainingLength -= glm::max(0, (int)element.children.size() - 1) * *properties.gap;
        for(Entity child : element.children)
        {
            remainingLength -= GetLengthWithAxis(child, *properties.direction);
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
        float length = GetLengthAgainstAxis(entity, *properties.direction);

        // Against axis padding (invert the direction to get the correct padding)
        length -= GetPadding(*properties.padding, (properties.direction == Direction::Horizontal ? Direction::Vertical : Direction::Horizontal));

        float largestLength = 0;
        for(Entity child : element.children)
        {
            float length = GetLengthAgainstAxis(child, *properties.direction);
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
            if(properties.direction == Direction::Horizontal)
            {
                localPos += glm::vec2(justifyContentOffset, alignItemsOffset);
                justifyContentOffset += childTransform.size.x + *properties.gap;
            }
            else
            {
                localPos += glm::vec2(alignItemsOffset, justifyContentOffset);
                justifyContentOffset += childTransform.size.y + *properties.gap;
            }

            // Assign the child's position relative to parent
            childTransform.position = localPos;

            // Recursively calculate children's positions
            CalculatePositions(child, localPos);
        }
    }
};

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
        auto [element, transform, layout, style, text] = entity.GetComponents<UIElement, UITransform, UILayout, UIStyle, Text>();

        SpriteCommand cmd = {.sprite = {NO_TEXTURE, style.color}, .transform = transform, .renderSpace = RenderSpace::Screen};
        Simplex::GetRendererManager().Submit<SpriteCommand>(cmd);

        if(!text.content.Get().empty())
        {
            glm::vec2 pos = transform.position;
            pos.x += layout.padding.Get().left;
            pos.y += layout.padding.Get().top;

            TextCommand cmd = {.text = text, .position = pos};
            Simplex::GetRendererManager().Submit<TextCommand>(cmd);
        }

        for(Entity child : element.children)
        {
            RenderElements(child);
        }
    }
};
