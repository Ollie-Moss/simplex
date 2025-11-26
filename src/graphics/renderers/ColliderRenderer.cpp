#include "graphics/renderers/ColliderRenderer.h"
#include "core/Simplex.h"
#include "core/View.h"
#include "glm/fwd.hpp"
#include "graphics/render-commands/ColliderCommand.h"
#include "graphics/util/RenderSpace.h"
#include "graphics/util/Shader.h"

void ColliderRenderer::Submit(const ColliderCommand &data)
{
    m_Buffer.Insert(data);
};

void ColliderRenderer::Render()
{
    for(size_t i = 0; i < m_Buffer.Size(); ++i)
    {
        switch(m_Buffer[i].collider.shape)
        {
        case Shape2D::Box:
            RenderRect(m_Buffer[i]);
            break;
        case Shape2D::Circle:
            RenderCircle(m_Buffer[i]);
            break;
        default:
            break;
        }
    }
    m_Buffer.Clear();
};

void ColliderRenderer::RenderCircle(const ColliderCommand &cmd)
{
    m_VertexArray.Bind();
    std::vector<glm::vec3> vertices;

    int segments = 20;
    vertices.reserve(segments);
    glm::vec3 position = cmd.transform.position + glm::vec3(cmd.collider.offset, 0.0f) + cmd.collider.radius;

    for(int i = 0; i < segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = cmd.collider.radius * cosf(theta);
        float y = cmd.collider.radius * sinf(theta);

        vertices.push_back(position + glm::vec3(x, y, 0));
    }
    m_VertexBuffer.Fill(vertices);
    m_VertexArray.Bind<glm::vec3>(0, &m_VertexBuffer);

    Shader shader = Simplex::GetResources().GetShader("DefaultShader");
    shader.use();

    // set color
    shader.setVec4("inColor", cmd.color);

    // set projection
    glm::mat4 projection;
    projection = Simplex::GetView().CalculateProjection(RenderSpace::World);
    shader.setMat4("projection", projection);

    glLineWidth(cmd.lineWidth);
    // render
    m_VertexArray.Render(segments, GL_LINE_LOOP);
    glBindTexture(GL_TEXTURE0, 0);
}

void ColliderRenderer::RenderRect(const ColliderCommand &cmd)
{
    m_VertexArray.Bind();

    std::vector<glm::vec3> vertices = {
        {0.0f, 0.0f, 0.0f}, // bottom-left
        {1.0f, 0.0f, 0.0f}, // bottom-right
        {1.0f, 1.0f, 0.0f}, // top-right
        {0.0f, 1.0f, 0.0f}, // top-left
    };

    glm::vec3 position = cmd.transform.position + glm::vec3(cmd.collider.offset, 0.0f);
    for(auto &vert : vertices)
    {
        vert *= glm::vec3(cmd.transform.size, 0.0f);
        vert += position;
    }

    m_VertexBuffer.Fill(vertices);
    m_VertexArray.Bind<glm::vec3>(0, &m_VertexBuffer);

    Shader shader = Simplex::GetResources().GetShader("DefaultShader");
    shader.use();

    // set color
    shader.setVec4("inColor", cmd.color);

    // set projection
    glm::mat4 projection;
    projection = Simplex::GetView().CalculateProjection(RenderSpace::World);
    shader.setMat4("projection", projection);

    glLineWidth(cmd.lineWidth);
    // render
    m_VertexArray.Render(4, GL_LINE_LOOP);
    glBindTexture(GL_TEXTURE0, 0);
}
