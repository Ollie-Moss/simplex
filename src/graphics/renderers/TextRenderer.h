#pragma once

#include <glm/glm.hpp>
#include "graphics/render-commands/TextCommand.h"
#include "graphics/renderers/IRenderer.h"
#include "graphics/util/Buffer.h"
#include "graphics/util/VertexArray.h"

class TextRenderer : public IRenderer<TextCommand>
{
  public:
    TextRenderer();
    void Submit(const TextCommand &data) override;
    void Render() override;

  private:
    void RenderText(const TextCommand &data);

  private:
    VertexBuffer m_VertexBuffer;
    VertexArray m_VertexArray;

    Buffer<TextCommand> m_Buffer;
};
