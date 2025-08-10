#include "Renderer2D.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "core/VertexArray.h"
#include "glm/fwd.hpp"
#include "graphics/QuadBuffer.h"
#include "graphics/RenderBuffer.h"
#include "graphics/Shader.h"

void Renderer2D::QueueWorldObject(Transform transform, std::string texture, Color color)
{
    RenderQueueData data = {.position = transform.position, .size = transform.size, .color = color, .texture = texture};

    if (!m_WorldRenderBuffer.has_value()) {
        m_WorldRenderBuffer.emplace();
    }
    m_WorldRenderBuffer.value().Insert(data);
}
void Renderer2D::Render()
{
    m_WorldRenderBuffer.value().Render();
}

void Renderer2D::RenderImmediate(Transform transform, std::string texture, Color color)
{
    VertexArray vao;
    Buffer vbo;
    QuadBuffer quadBuffer;

    RenderData data = {.position = transform.position, .size = transform.size, .color = color};
    vbo.Fill<RenderData>(data);

    vao.BindProperty<RenderData, glm::vec3>(0, offsetof(RenderData, position) / sizeof(float), &vbo);
    vao.BindProperty<RenderData, glm::vec2>(1, offsetof(RenderData, size) / sizeof(float), &vbo);
    vao.BindProperty<RenderData, glm::vec4>(2, offsetof(RenderData, color) / sizeof(float), &vbo);
    vao.AttributeDivisor(0, 1);
    vao.AttributeDivisor(1, 1);
    vao.AttributeDivisor(2, 1);
    vao.Bind<glm::vec3>(3, &quadBuffer);

    Shader shader = Simplex::GetResources().GetShader("SpriteShader");
    shader.use();

    // set projection
    glm::mat4 projection = Simplex::GetView().CalculateScreenSpaceProjection();
    shader.setMat4("projection", projection);

    // set texture
    bool useTexture = (texture != "");
    shader.setBool("useTexture", useTexture);

    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);
        Simplex::GetResources().GetTexture(texture).Bind();
    }

    vao.RenderInstanced(6, 1);
    glBindTexture(GL_TEXTURE0, 0);
}

void Renderer2D::RenderText(std::string text, glm::vec2 position, glm::vec2 size, glm::vec4 color, std::string fontName)
{
    VertexArray vao;
    Buffer vert;
    vao.Bind<glm::vec4>(0, &vert);

    Font font = Simplex::GetResources().GetFont(fontName);
    Shader shader = Simplex::GetResources().GetShader("TextShader");
    shader.use();

    shader.setVec3("textColor", color);
    glm::mat4 projection = Simplex::GetView().CalculateScreenSpaceProjection();
    shader.setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    vao.Bind();

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = font.characters[*c];

        float xpos = position.x + ch.Bearing.x * size.x;
        float ypos = position.y - (ch.Size.y - ch.Bearing.y) * size.y;

        float w = ch.Size.x * size.x;
        float h = ch.Size.y * size.y;

        // update VBO for each character
        float vertices[6][4] = {{xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f},     {xpos + w, ypos, 1.0f, 1.0f},

                                {xpos, ypos + h, 0.0f, 0.0f}, {xpos + w, ypos, 1.0f, 1.0f}, {xpos + w, ypos + h, 1.0f, 0.0f}};

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        vert.Fill(vertices);

        // render quad
        vao.Render(6, GL_TRIANGLES);

        position.x += (ch.Advance >> 6) * size.x;  // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
