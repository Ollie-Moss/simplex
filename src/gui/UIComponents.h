#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "components/Transform.h"
#include "core/Types.h"
#include "glm/fwd.hpp"

enum class AlignItems { Start, End, Center, Stretch };
enum class JustifyContent { Start, End, Center, SpaceBetween, SpaceAround };
enum class FlexDirection { Row, Column };

struct UIElement {
    EntityId parent = -1;
    std::vector<EntityId> children;
};

struct UIProperties {
    FlexDirection direction = FlexDirection::Row;
    Padding padding;
    Color color = BLUE;
    float gap = 8.0f;
    AlignItems alignItems = AlignItems::Start;
    JustifyContent justifyContent = JustifyContent::Start;
};

struct UISize {
    float width;  // Could support auto, %, px
    float height;
};

struct UITransform {
    glm::vec2 position = glm::vec2(0, 0);
    glm::vec2 size = glm::vec2(100, 100);

    operator Transform()
    {
        return Transform{.position = glm::vec3(position.x, position.y, 0), .size = size};
    }
};
