#pragma once

// This component describes the actual rendered dimensions of a given elements
// These values are calculated by the UI layout systems based on the UILayout properties
struct UITransform
{
    glm::vec2 position = glm::vec2(0, 0);
    glm::vec2 size = glm::vec2(10, 10);

    operator Transform()
    {
        return Transform{.position = glm::vec3(position.x, position.y, 0),
                         .size = size};
    }
    bool operator==(const UITransform &rhs) const = default;
};
