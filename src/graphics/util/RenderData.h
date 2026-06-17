#pragma once

#include "glm/glm.hpp"
#include "graphics/render-commands/SpriteCommand.h"

struct RenderData
{
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec2 size = glm::vec2(0, 0);
    glm::vec4 color = glm::vec4(0, 0, 0, 0);
    glm::vec2 texturePosition = glm::vec2(0, 0);
    glm::vec2 textureSize = glm::vec2(0, 0);

    RenderData() {}
    RenderData(const SpriteCommand &spriteCommand)
    {
        position = spriteCommand.transform.position;
        size = spriteCommand.transform.size;
        color = spriteCommand.sprite.color;
    }
};
