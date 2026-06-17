#pragma once

#include "components/Sprite.h"
#include "graphics/util/RenderSpace.h"

struct SpriteCommand
{
    Sprite sprite;
    Transform transform;
    RenderSpace renderSpace;
};
