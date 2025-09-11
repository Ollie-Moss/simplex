#pragma once

#include "components/Transform.h"
#include "glm/fwd.hpp"
#include "graphics/IRenderer.h"

struct Quad
{
    Transform transform;
    glm::vec4 color = glm::vec4(1, 1, 1, 1);
};

class QuadRenderer : public IRenderer<Quad>
{
    void Submit(const Quad &data) override;
};
