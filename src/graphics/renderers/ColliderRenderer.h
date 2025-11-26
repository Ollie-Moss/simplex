#pragma once

#include "graphics/render-commands/ColliderCommand.h"
#include "graphics/util/VertexArray.h"
#include "graphics/util/Buffer.h"
#include "graphics/renderers/IRenderer.h"

class ColliderRenderer : public IRenderer<ColliderCommand>
{
  public:
    void Submit(const ColliderCommand &data) override;
    void Render() override;

  private:
    void RenderCircle(const ColliderCommand &data);
    void RenderRect(const ColliderCommand &data);

  private:
    VertexArray m_VertexArray;
    VertexBuffer m_VertexBuffer;

    Buffer<ColliderCommand> m_Buffer;
};
