#pragma once

#include "components/Transform.h"
#include "core/Types.h"
#include "physics/Collider2D.h"

struct ColliderCommand
{
    Transform transform;
    Collider2D collider;
    Color color = RED;
    float lineWidth = 2.0f;
};
