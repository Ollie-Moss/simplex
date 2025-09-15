#pragma once

#include "graphics/util/QuadBuffer.h"
#include "graphics/util/VertexArray.h"
#include "graphics/util/Buffer.h"
#include "graphics/util/PositionSortedBuffer.h"
#include "graphics/renderers/IRenderer.h"
#include "graphics/render-commands/SpriteCommand.h"

class SpriteRenderer : public IRenderer<SpriteCommand>
{
  public:
    void Submit(const SpriteCommand &data) override;
    void Render() override;

  private:
    void RenderRange(Buffer<SpriteCommand> *buffer, const size_t &rangeStart, const size_t &rangeEnd);
    void RenderBuffer(Buffer<SpriteCommand> *buffer);

  private:
    VertexArray m_VertexArray;
    VertexBuffer m_InstanceBuffer;
    QuadVertexBuffer m_QuadBuffer;

    Buffer<SpriteCommand> m_ScreenBuffer;
    PositionSortedBuffer m_WorldBuffer;
};
