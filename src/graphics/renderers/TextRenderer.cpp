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
    const Shader &shader = Simplex::GetAssetManager().Get<Shader>("TextShader");
    shader.use();
    glm::mat4 projection = Simplex::GetView().CalculateProjection(RenderSpace::Screen);
    shader.setMat4("projection", projection);
    for(size_t i = 0; i < m_Buffer.Size(); ++i)
    {
        RenderText(m_Buffer[i], shader);
    }
    m_Buffer.Clear();
}

void TextRenderer::RenderText(const TextCommand &data, const Shader &shader)
{
    shader.setVec3("textColor", data.color);
    shader.setVec4("clipRect", data.clipRect);

    for(auto &glyph : data.glyphs)
    {
        glm::vec2 pos = glm::vec2(glyph.transform.position) + data.position;
        float w = glyph.transform.size.x;
        float h = glyph.transform.size.y;

        // update VBO for each character
        glm::vec4 vertices[6] = //
            {
                glm::vec4(pos.x, pos.y, 0.0f, 0.0f),         //
                glm::vec4(pos.x, pos.y + h, 0.0f, 1.0f),     //
                glm::vec4(pos.x + w, pos.y + h, 1.0f, 1.0f), //
                glm::vec4(pos.x, pos.y, 0.0f, 0.0f),         //
                glm::vec4(pos.x + w, pos.y + h, 1.0f, 1.0f), //
                glm::vec4(pos.x + w, pos.y, 1.0f, 0.0f)      //
            }; //

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, glyph.texture);
        m_VertexBuffer.Fill<glm::vec4>(6, &vertices[0]);

        // render quad
        m_VertexArray.Render(6, GL_TRIANGLES);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
