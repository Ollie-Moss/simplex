#include "graphics/TextRenderer.h"
#include "glm/fwd.hpp"
#include "graphics/Font.h"
#include "core/Simplex.h"
#include "graphics/Shader.h"

TextRenderer::TextRenderer()
{
    m_Buffer.Fill<float>(static_cast<size_t>(6 * 4), nullptr);
    m_VertexArray.BindProperty<glm::vec4, glm::vec4>(0, 0, &m_Buffer);
}
void TextRenderer::RenderText(std::string text, glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontName)
{
    Font font = Simplex::GetResources().GetFont(fontName);
    Shader shader = Simplex::GetResources().GetShader("TextShader");
    shader.use();

    shader.setVec3("textColor", color);
    glm::mat4 projection = Simplex::GetView().CalculateScreenSpaceProjection();
    shader.setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    m_VertexArray.Bind();

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = font.characters[*c];

        float xpos = position.x + ch.Bearing.x * size.x;
        float ypos = position.y - ch.Bearing.y;

        float w = ch.Size.x * size.x;
        float h = ch.Size.y * size.y;

        // update VBO for each character
        glm::vec4 vertices[6] =  //
            {
                glm::vec4(xpos, ypos, 0.0f, 0.0f),          //
                glm::vec4(xpos, ypos + h, 0.0f, 1.0f),      //
                glm::vec4(xpos + w, ypos + h, 1.0f, 1.0f),  //
                glm::vec4(xpos, ypos, 0.0f, 0.0f),          //
                glm::vec4(xpos + w, ypos + h, 1.0f, 1.0f),  //
                glm::vec4(xpos + w, ypos, 1.0f, 0.0f)       //
            };  //

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        m_Buffer.Fill<glm::vec4>(6, &vertices[0]);

        // render quad
        m_VertexArray.Render(6, GL_TRIANGLES);

        position.x += (ch.Advance >> 6) * size.x;  // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
