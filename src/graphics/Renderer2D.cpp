#include "Renderer2D.h"
#include "core/Simplex.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <vector>

void Renderer2D::Queue(Sprite sprite) {
    m_Textures.insert(sprite.texture);

    m_Positions[sprite.texture].push_back(sprite.position);
    m_Sizes[sprite.texture].push_back(sprite.size);
    m_Colors[sprite.texture].push_back(sprite.color);
}

void Renderer2D::Render() {

    for (auto t : m_Textures) {
        m_Buffers[t][0].Fill<glm::vec3>(m_Positions[t]);
        m_Buffers[t][1].Fill<glm::vec2>(m_Sizes[t]);
        m_Buffers[t][2].Fill<glm::vec4>(m_Colors[t]);

        m_VertexArrays[t].Bind<glm::vec3>(0, &m_Buffers[t][0]);
        m_VertexArrays[t].Bind<glm::vec2>(1, &m_Buffers[t][1]);
        m_VertexArrays[t].Bind<glm::vec4>(2, &m_Buffers[t][2]);

        // set shader and texture
        Shader shader = Simplex::GetResources().GetShader("SpriteShader");
        shader.use();

        glm::mat4 projection = Simplex::GetView().CalculateWorldSpaceProjection();
        shader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        Simplex::GetResources().GetTexture(t).Bind();

        m_VertexArrays[t].RenderInstanced(static_cast<int>(m_Positions[t].size()));
        glBindTexture(GL_TEXTURE0, 0);
    }

    // Clear the buffers
    for (auto t : m_Textures) {
        m_Positions[t].clear();
        m_Sizes[t].clear();
        m_Colors[t].clear();
    }
    m_Textures.clear();
}
