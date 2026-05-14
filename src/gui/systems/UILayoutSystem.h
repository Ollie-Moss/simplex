#pragma once

#include "core/Simplex.h"
#include "graphics/text/Character.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "graphics/text/Font.h"
#include "gui/components/UIElement.h"
#include "gui/components/UITransform.h"
#include "gui/components/UILayout.h"
#include "gui/components/Text.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <sys/types.h>
#include <vector>

class UILayoutSystem : public System
{
  public:
    UILayoutSystem(Registry &registry, const SimplexModules &modules) : System(registry, modules)
    {
        m_Signature = m_Registry.CreateSignature<UIElement, UITransform, UILayout, Text>();
    }
    void Update(float timeStep) override
    {
        for(EntityId e : m_Entities)
        {
            UIElement &element = m_Registry.GetComponent<UIElement>(e);
            if(element.parent != NULL_ENTITY)
                continue;

            CalculateLayout(e);
            // if(IsDirty(e))
            // {
            // }
        }
    }

    bool IsDirty(EntityId entity)
    {
        UIElement &element = m_Registry.GetComponent<UIElement>(entity);
        if(element.dirty || Simplex::GetView().HasWindowResized())
        {
            return true;
        }

        for(EntityId e : element.children)
        {
            if(IsDirty(e))
            {
                return true;
            }
        }
        return false;
    }

    void CalculateLayout(EntityId entity)
    {
        UIElement &elem = m_Registry.GetComponent<UIElement>(entity);
        UITransform &transform = m_Registry.GetComponent<UITransform>(entity);

        bool shouldWrap = false;
        TextSize(entity, shouldWrap);
        InitialSizing(entity, Direction::Horizontal);
        InitialSizing(entity, Direction::Vertical);

        // HUG Sizing width
        HugSize(entity, Direction::Horizontal);

        // GROW Sizing width
        GrowSize(entity, Direction::Horizontal);

        // Wrap Text
        shouldWrap = true;
        TextSize(entity, shouldWrap);

        // HUG Sizing heights
        HugSize(entity, Direction::Vertical);

        // GROW Sizing heights
        GrowSize(entity, Direction::Vertical);

        // Calculate Positions
        CalculatePositions(entity, glm::vec2(0.0f));

        // Marks all elements NOT dirty
        CleanTree(entity);
    }

    void CleanTree(EntityId entity)
    {
        UIElement &elem = m_Registry.GetComponent<UIElement>(entity);
        elem.dirty = false;

        for(EntityId e : elem.children)
        {
            CleanTree(e);
        }
    }

    const Axis &GetAxis(EntityId entity, Direction direction)
    {
        UILayout &props = m_Registry.GetComponent<UILayout>(entity);
        if(direction == Direction::Horizontal)
        {
            return props.sizing.width;
        }
        return props.sizing.height;
    }
    float GetPadding(const Padding &padding, Direction direction)
    {
        if(direction == Direction::Horizontal)
        {
            return padding.left + padding.right;
        }
        return padding.top + padding.bottom;
    }
    float GetParentPadding(EntityId entity, Direction direction)
    {
        UIElement &element = m_Registry.GetComponent<UIElement>(entity);
        UILayout &properties = m_Registry.GetComponent<UILayout>(entity);
        UITransform &transform = m_Registry.GetComponent<UITransform>(entity);

        EntityId parent = element.parent;
        if(parent == NULL_ENTITY)
        {
            return 0.0f;
        }
        UIElement &parentElement = m_Registry.GetComponent<UIElement>(parent);
        UILayout &parentProperties = m_Registry.GetComponent<UILayout>(parent);
        return GetPadding(parentProperties.padding, direction);
    }

    float &GetLengthWithAxis(EntityId entity, Direction direction)
    {
        if(direction == Direction::Horizontal)
        {
            return m_Registry.GetComponent<UITransform>(entity).size.x;
        }
        return m_Registry.GetComponent<UITransform>(entity).size.y;
    }
    float &GetLengthAgainstAxis(EntityId entity, Direction direction)
    {
        if(direction != Direction::Horizontal)
        {
            return m_Registry.GetComponent<UITransform>(entity).size.x;
        }
        return m_Registry.GetComponent<UITransform>(entity).size.y;
    }

    float GetLargestChildLength(EntityId entity, Direction direction)
    {
        float largest = 0.0f;
        for(EntityId child : m_Registry.GetComponent<UIElement>(entity).children)
        {
            float length = GetLengthWithAxis(child, direction);
            if(length > largest)
            {
                largest = length;
            }
        }
        return largest;
    }

    float SumChildrenLengths(EntityId entity, Direction direction)
    {
        float total = 0.0f;
        for(EntityId child : m_Registry.GetComponent<UIElement>(entity).children)
        {
            total += GetLengthWithAxis(child, direction);
        }
        return total;
    }

    std::vector<EntityId> GetGrowableChildren(EntityId entity, Direction direction)
    {
        std::vector<EntityId> growables;
        for(EntityId child : m_Registry.GetComponent<UIElement>(entity).children)
        {
            if(GetAxis(child, direction).mode == SizingMode::Grow)
            {
                growables.push_back(child);
            }
        }
        return growables;
    }

    void TextSize(EntityId entity, bool shouldWrap)
    {
        Text &text = m_Registry.GetComponent<Text>(entity);
        TextLayout &textLayout = m_Registry.GetComponent<TextLayout>(entity);
        UITransform &transform = m_Registry.GetComponent<UITransform>(entity);
        UIElement &elem = m_Registry.GetComponent<UIElement>(entity);
        UILayout &layout = m_Registry.GetComponent<UILayout>(entity);

        for(EntityId e : elem.children)
        {
            TextSize(e, shouldWrap);
        }
        if(text.content.empty())
            return;

        float maxWidth = transform.size.x - layout.padding.right;

        // Skip recalculation if text size is smaller max width (ONLY IF WRAPPING IS BEING CALCULATED)
        if(textLayout.size.x + layout.padding.left <= maxWidth && shouldWrap)
            return;

        // do some wrapping
        Font font = Simplex::GetAssetManager().Get<Font>(text.fontName);

        float penX = 0.0f;
        float penY = font.maxAscent;
        float lineWidth = 0.0f;
        float lineHeight = 0.0f;

        float accumulatedHeight = penY;
        float bottomLineDescent = 0.0f;

        float largestLineWidth = 0.0f;

        int numOfLines = 0;

        std::vector<GlyphQuad> glyphs;
        std::vector<float> lineWidths;

        auto newLine = [&]() {
            lineWidths.push_back(lineWidth);
            accumulatedHeight += font.lineHeight;

            penX = 0;
            penY += font.lineHeight * text.lineHeight;
            lineWidth = 0.0f;
            lineHeight = 0.0f;
            numOfLines++;
        };

        auto insertChar = [&](Character ch) {
            // Update lineWidth and
            lineWidth = penX + ch.Size.x;
            if(lineWidth > largestLineWidth)
                largestLineWidth = lineWidth;

            // Create Glyph Quad
            GlyphQuad glyph;
            glyph.descent = ch.Size.y - ch.Bearing.y;
            glyph.ascent = ch.Size.y - glyph.descent;
            glyph.advance = ch.Advance;
            glyph.transform = {glm::vec3{penX + layout.padding.left, penY + layout.padding.top - ch.Bearing.y, 0}, ch.Size};
            glyph.texture = ch.TextureID;
            glyphs.push_back(glyph);

            // Advance
            penX += ch.Advance;
        };

        for(size_t i = 0; i < text.content.size(); i++)
        {
            char c = text.content[i];
            Character ch = font.characters[c];

            // newline if '\n' found or we exceed the width of the container
            if(c == '\n')
            {
                // Save data
                newLine();
                continue;
            }
            if(c == '\t')
            {
                for(int j = 0; j < TABWIDTH; ++j)
                    insertChar(font.characters[' ']);

                continue;
            }

            // Wrap text if text size is larger max width (ONLY IF WRAPPING IS BEING CALCULATED)
            if(penX + ch.Size.x + layout.padding.left > maxWidth && shouldWrap)
            {
                newLine();
                numOfLines++;
            }

            insertChar(ch);
        }

        // Create text bounds and update text layout
        float width = largestLineWidth;
        float height = accumulatedHeight + font.maxDescent;

        textLayout.size = glm::vec2{width, height};
        textLayout.glyphs = glyphs;
        textLayout.lineWidths = lineWidths;
    }
    void InitialSizing(EntityId entity, Direction sizingAxis)
    {
        UIElement &element = m_Registry.GetComponent<UIElement>(entity);
        UILayout &properties = m_Registry.GetComponent<UILayout>(entity);
        UITransform &transform = m_Registry.GetComponent<UITransform>(entity);
        Text &text = m_Registry.GetComponent<Text>(entity);
        TextLayout &textLayout = m_Registry.GetComponent<TextLayout>(entity);

        float &length = GetLengthWithAxis(entity, sizingAxis);
        Axis axis = GetAxis(entity, sizingAxis);

        auto padding = GetPadding(properties.padding, sizingAxis);

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

        if(!text.content.empty() && sizingAxis == Direction::Horizontal && axis.mode != SizingMode::Fixed)
        {
            float textLength = sizingAxis == Direction::Horizontal ? textLayout.size.x : textLayout.size.y;
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
    void HugSize(EntityId entity, Direction sizingAxis)
    {
        UIElement &element = m_Registry.GetComponent<UIElement>(entity);
        UILayout &properties = m_Registry.GetComponent<UILayout>(entity);
        UITransform &transform = m_Registry.GetComponent<UITransform>(entity);
        Text &text = m_Registry.GetComponent<Text>(entity);
        TextLayout &textLayout = m_Registry.GetComponent<TextLayout>(entity);

        for(auto child : element.children)
        {
            HugSize(child, sizingAxis);
        }

        if(GetAxis(entity, sizingAxis).mode != SizingMode::Hug)
            return;

        float gap = glm::max(0, (int)element.children.size() - 1) * properties.gap;
        float padding = GetPadding(properties.padding, sizingAxis);

        float &length = GetLengthWithAxis(entity, sizingAxis);
        float textSize = sizingAxis == Direction::Horizontal ? textLayout.size.x : textLayout.size.y;
        length = padding + gap + textSize;

        // Size with layout direction
        if(sizingAxis == properties.direction)
        {
            length += SumChildrenLengths(entity, sizingAxis);
        }
        else
        {
            // Size across layout direction
            length += GetLargestChildLength(entity, sizingAxis);
        }
    }

    void GrowSize(EntityId entity, Direction sizingAxis)
    {
        UIElement &element = m_Registry.GetComponent<UIElement>(entity);
        UILayout &properties = m_Registry.GetComponent<UILayout>(entity);
        UITransform &transform = m_Registry.GetComponent<UITransform>(entity);

        if(element.parent == NULL_ENTITY && GetAxis(entity, sizingAxis).mode == SizingMode::Grow)
        {
            float &length = GetLengthWithAxis(entity, sizingAxis);
            length = (sizingAxis == Direction::Vertical ? Simplex::GetView().GetWindowHeight() : Simplex::GetView().GetWindowWidth());
        }
        std::vector<EntityId> growables = GetGrowableChildren(entity, sizingAxis);
        if(growables.empty())
        {
            for(EntityId child : element.children)
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
            for(EntityId child : growables)
            {
                float &length = GetLengthWithAxis(child, sizingAxis);

                length = remainingLength;
            }
            for(EntityId child : element.children)
            {
                GrowSize(child, sizingAxis);
            }
            return;
        }

        // Size with layout direction
        remainingLength -= glm::max(0, (int)element.children.size() - 1) * properties.gap;

        for(EntityId child : element.children)
        {
            remainingLength -= GetLengthWithAxis(child, sizingAxis);
        }
        ResizeChildren(entity, growables, remainingLength, sizingAxis);

        for(EntityId child : element.children)
        {
            GrowSize(child, sizingAxis);
        }
    }

    void ResizeChildren(EntityId parent, std::vector<EntityId> growables, float remainingLength, Direction sizingAxis)
    {
        const float EPSILON = 0.01f;
        int maxIterations = 100;

        while(std::abs(remainingLength) > EPSILON && maxIterations-- > 0)
        {
            //  Calculate total length of growables
            float totalLength = 0.0f;
            for(EntityId child : growables)
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
            for(EntityId child : growables)
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

    void CalculatePositions(EntityId entity, glm::vec2 parentPosition)
    {
        UIElement &element = m_Registry.GetComponent<UIElement>(entity);
        UILayout &properties = m_Registry.GetComponent<UILayout>(entity);
        UITransform &transform = m_Registry.GetComponent<UITransform>(entity);
        TextLayout &textLayout = m_Registry.GetComponent<TextLayout>(entity);

        EntityId parent = element.parent;

        float justifyContentOffset = (properties.direction == Direction::Horizontal) ? properties.padding.left : properties.padding.top;
        float alignItemsOffset = (properties.direction == Direction::Vertical) ? properties.padding.left : properties.padding.top;

        // Apply justify content positions
        float remainingLength = GetLengthWithAxis(entity, properties.direction);

        // remainingLength -= GetParentPadding(properties.padding, direction);
        remainingLength -= GetPadding(properties.padding, properties.direction);
        remainingLength -= glm::max(0, (int)element.children.size() - 1) * properties.gap;
        for(EntityId child : element.children)
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
        length -= GetPadding(properties.padding, (properties.direction == Direction::Horizontal ? Direction::Vertical : Direction::Horizontal));

        float largestLength = 0;
        for(EntityId child : element.children)
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
        for(EntityId child : element.children)
        {
            UITransform &childTransform = m_Registry.GetComponent<UITransform>(child);

            glm::vec2 localPos = parentPosition;
            if(properties.direction == Direction::Horizontal)
            {
                localPos += glm::vec2(justifyContentOffset, alignItemsOffset);
                localPos.x += textLayout.size.x;
                justifyContentOffset += childTransform.size.x + properties.gap;
            }
            else
            {
                localPos += glm::vec2(alignItemsOffset, justifyContentOffset);
                localPos.y += textLayout.size.y;
                justifyContentOffset += childTransform.size.y + properties.gap;
            }

            // Assign the child's position relative to parent
            childTransform.position = localPos;

            // Recursively calculate children's positions
            CalculatePositions(child, localPos);
        }
    }
};
