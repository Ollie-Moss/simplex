#pragma once

#include "core/SystemManager.h"
#include "core/Input.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "graphics/render-commands/ColliderCommand.h"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/text/Font.h"
#include "graphics/util/RenderMode.h"
#include "graphics/util/RenderSpace.h"
#include "gui/Text.h"
#include "gui/UIComponents.h"
#include <algorithm>
#include <format>
#include <iostream>

class UIInputSystem : public System
{
  public:
    UIInputSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<UIElement, UITransform, Text, UIEvents, UIInput>();
    }
    void Update(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            auto [elem, tranform, text, event, input] = e.GetComponents<UIElement, UITransform, Text, UIEvents, UIInput>();

            // if(!input.focused || input.readOnly)
            //     continue;

            // Text Input
            std::string inputText = Simplex::GetInput().GetTextInput();
            bool highlight = Simplex::GetInput().GetKey(GLFW_KEY_LEFT_SHIFT).IsDown();
            bool moveByWord = Simplex::GetInput().GetKey(GLFW_KEY_LEFT_CONTROL).IsDown();

            // modifiers
            int horizontalAxis = Simplex::GetInput().GetKey(GLFW_KEY_LEFT).IsPressedOrRepeating() * -1 + Simplex::GetInput().GetKey(GLFW_KEY_RIGHT).IsPressedOrRepeating();
            int verticalAxis = Simplex::GetInput().GetKey(GLFW_KEY_DOWN).IsPressed() * -1 + Simplex::GetInput().GetKey(GLFW_KEY_UP).IsDown();

            // if ctrl is pressed action distance should be the full word

            // Horizontal movement
            int actionDistance = 1;
            if(moveByWord && horizontalAxis != 0)
            {
                actionDistance = GetActionDistance(input, horizontalAxis);
            }
            input.cursor = std::clamp<int>(input.cursor + horizontalAxis * actionDistance, 0, input.value.length());
            // highlighted text gets replaced

            // start at cursor if not highlighted

            for(char c : inputText)
            {
                if(c == '\b')
                {
                    if(input.value.size() > 0)
                    {
                        int amount = 1;
                        if(moveByWord)
                            amount = GetActionDistance(input, -1);
                        input.value.erase(input.cursor - amount, amount);
                        input.cursor -= amount;
                    }
                    continue;
                }
                input.value.insert(input.value.begin() + input.cursor, c);

                input.cursor++;
            }
            text.content = input.value;
            elem.dirty = true;
        }
    }

    int GetActionDistance(UIInput &input, int direction)
    {
        int actionDistance = 1;
        for(int i = input.cursor + direction; i <= input.value.length() && i >= -1; i += direction)
        {
            int index = i + direction;
            if(index < 0 || index > input.value.length() - 1)
                break;

            if(input.value[index] == ' ')
                break;

            actionDistance++;
        }
        return actionDistance;
    }
};

class UIInputRenderSystem : public System
{
  public:
    UIInputRenderSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<UIElement, UITransform, Text, TextLayout, UIEvents, UIInput>();
    }
    void Update(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            auto [layout, elem, transform, text, textLayout, event, input] = e.GetComponents<UILayout, UIElement, UITransform, Text, TextLayout, UIEvents, UIInput>();
            Font font = Simplex::GetAssetManager().Get<Font>(text.fontName);

            int glyphIndex = input.cursor - (textLayout.glyphs.size() - text.content.Get().length());
            if(glyphIndex > textLayout.glyphs.size() || textLayout.glyphs.size() < 1)
                continue;

            glm::vec3 pos = glm::vec3(transform.position, 0);
            glm::vec2 size = {1, font.lineHeight};
            GlyphQuad glyph;
            if(glyphIndex == textLayout.glyphs.size())
            {
                glyph = textLayout.glyphs[glyphIndex - 1];
                pos.x += glyph.advance;
            }
            else
            {
                glyph = textLayout.glyphs[glyphIndex];
            }

            pos += glm::vec3{glyph.transform.position.x, glyph.transform.position.y + glyph.ascent - font.maxAscent, 0};
            size.x = glyph.transform.size.x;

            Simplex::GetRendererManager().Submit<SpriteCommand>({.sprite = {NO_TEXTURE, BLACK}, .transform = {pos, size}, .renderSpace = RenderSpace::Screen, .renderMode = RenderMode::Queue});
        }
    }
};
