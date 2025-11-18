#pragma once

#include "components/Sprite.h"
#include "graphics/util/RenderData.h"
#include "graphics/util/RenderMode.h"
#include "graphics/util/RenderSpace.h"

struct SpriteCommand
{
    Sprite sprite;
    Transform transform;
    RenderSpace renderSpace;
    RenderMode renderMode = RenderMode::Queue;
    operator RenderData() const
    {
        return {transform.position, transform.size, sprite.color};
    }
};
