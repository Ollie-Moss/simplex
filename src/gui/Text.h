#pragma once

#include <string>
#include "Bindable.h"
#include "glm/fwd.hpp"
#include "graphics/text/Font.h"

struct Text
{
    std::string fontName = "Arial";
    Bindable<std::string> content = std::string("");
    float fontSize = 12;
    Color color = BLACK;
    float lineHeight = 20.0f;

    std::vector<int> breaks;

    auto bindables()
    {
        return std::tie(content);
    }

    bool operator==(const Text &rhs) const = default;

    glm::vec2 Measure()
    {
        Font font = Simplex::GetAssetManager().Get<Font>(fontName);

        float width = 0.0f;
        float height = 0.0f;
        float currentWidth = 0.0f;
        float largestWidth = 0.0f;
        float largestHeight = 0.0f;

        uint32_t lineCounter = 1;
        std::string textContent = content.Get();

        int inserted = 0;

        for(auto breakIndex : breaks)
        {
            if(breakIndex > textContent.size() - 1)
                continue;

            textContent.insert(breakIndex + inserted, 1, '\n');
            inserted++;
        }

        int lastLineStartingIndex = 0;
        for(int i = 0; i < textContent.length(); i++)
        {
            char c = textContent[i];
            Character ch = font.characters[c];

            // The height of a character is the ch.Size.y + any overhanging parts, only if it is the bottom-most line
            float chHeight = ch.Size.y;

            if(c == '\n')
            {
                lineCounter += 1;
                largestWidth = std::max(currentWidth, largestWidth);
                currentWidth = 0;

                if(i + 1 < textContent.length())
                {
                    lastLineStartingIndex = i + 1;
                }
            }
            else
            {
                currentWidth += (ch.Advance >> 6);
            }

            if(chHeight > largestHeight)
            {
                largestHeight = chHeight;
            }
        }

        for(int i = lastLineStartingIndex; i < textContent.length(); i++)
        {
            char c = textContent[i];
            Character ch = font.characters[c];

            float chHeight = ch.Size.y + (ch.Size.y - ch.Bearing.y);

            if(chHeight > largestHeight)
            {
                largestHeight = chHeight;
            }
        }

        width = std::max(currentWidth, largestWidth);

        float lineGaps = std::max<int>(0, lineCounter - 1) * lineHeight;
        float lineHeights = (lineCounter * largestHeight);
        height = lineHeights + lineGaps;

        return glm::vec2(width, height);
    }
};
