#include "graphics/renderers/TextRenderer.h"
#include "glm/fwd.hpp"
#include "graphics/render-commands/TextCommand.h"
#include "graphics/text/Font.h"
#include "core/Simplex.h"
#include "graphics/util/Buffer.h"
#include "graphics/util/Shader.h"
#include "graphics/util/RenderSpace.h"

TextRenderer::TextRenderer()
{
    m_VertexBuffer.Fill<float>(static_cast<size_t>(6 * 4), nullptr);
    m_VertexArray.BindProperty<glm::vec4, glm::vec4>(0, 0, &m_VertexBuffer);
}

void TextRenderer::Submit(const TextCommand &data)
{
    m_Buffer.Insert(data);
}

void TextRenderer::Render()
{
    for(size_t i = 0; i < m_Buffer.Size(); ++i)
    {
        RenderText(m_Buffer[i]);
    }
    m_Buffer.Clear();
}

void TextRenderer::RenderText(const TextCommand &data)
{
    Font font = Simplex::GetResources().GetFont(data.text.fontName);
    Shader shader = Simplex::GetResources().GetShader("TextShader");
    shader.use();

    shader.setVec3("textColor", data.text.color);
    glm::mat4 projection = Simplex::GetView().CalculateProjection(RenderSpace::Screen);
    shader.setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    m_VertexArray.Bind();

    // iterate through all characters
    std::string::const_iterator c;
    glm::vec2 position = data.position;
    float lineHeightPadding = 20.0f;

    float tallestChar = 0.0f;

    std::string text = *data.text.content;

    int inserted = 0;

    for(auto breakIndex : data.text.breaks)
    {
        if(breakIndex > text.size() - 1)
            continue;

        text.insert(breakIndex + inserted, 1, '\n');
        inserted++;
    }

    for(c = text.begin(); c != text.end(); c++)
    {
        Character ch = font.characters[*c];
        float height = ch.Size.y;
        if(height > tallestChar)
        {
            tallestChar = height;
        }
    }

    char lastChar;
    for(c = text.begin(); c != text.end(); c++)
    {
        // Skip spaces that start on new lines (this should potentially only be breaks defined in text.breaks)
        if(lastChar == '\n' && *c == ' ')
        {
            continue;
        }
        lastChar = *c;

        // Reset position for newlines
        if(*c == '\n')
        {
            position.y += tallestChar + lineHeightPadding;
            position.x = data.position.x;
            continue;
        }

        // Setup character position and bounds
        Character ch = font.characters[*c];

        float xpos = position.x + ch.Bearing.x;
        float ypos = position.y - ch.Bearing.y;
        ypos += tallestChar;

        float w = ch.Size.x;
        float h = ch.Size.y;

        // update VBO for each character
        glm::vec4 vertices[6] = //
            {
                glm::vec4(xpos, ypos, 0.0f, 0.0f),         //
                glm::vec4(xpos, ypos + h, 0.0f, 1.0f),     //
                glm::vec4(xpos + w, ypos + h, 1.0f, 1.0f), //
                glm::vec4(xpos, ypos, 0.0f, 0.0f),         //
                glm::vec4(xpos + w, ypos + h, 1.0f, 1.0f), //
                glm::vec4(xpos + w, ypos, 1.0f, 0.0f)      //
            }; //

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        m_VertexBuffer.Fill<glm::vec4>(6, &vertices[0]);

        // render quad
        m_VertexArray.Render(6, GL_TRIANGLES);

        position.x += (ch.Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
