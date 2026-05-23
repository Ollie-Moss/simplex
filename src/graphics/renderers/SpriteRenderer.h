#pragma once

#include "graphics/util/QuadBuffer.h"
#include "graphics/util/RenderData.h"
#include "graphics/util/Shader.h"
#include "graphics/util/VertexArray.h"
#include "graphics/renderers/IRenderer.h"
#include "graphics/render-commands/SpriteCommand.h"
#include <vector>

class SpriteRenderer : public IRenderer<SpriteCommand>
{
  public:
    SpriteRenderer();

    void Submit(const SpriteCommand &data) override;
    void Render() override;

  private:
    void RenderRange(const std::vector<SpriteCommand> &buffer, const size_t &rangeStart, const size_t &rangeEnd);
    void RenderBuffer(const std::vector<RenderData> &buffer);

  private:
    VertexArray m_VertexArray;
    VertexBuffer m_InstanceBuffer;
    QuadVertexBuffer m_QuadBuffer;

    std::vector<RenderData> m_ScreenBuffer;
    std::vector<RenderData> m_WorldBuffer;

    Shader m_Shader;
    glm::mat4 m_Projection;
};
