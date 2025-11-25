#include "glm/glm.hpp"

enum class Shape2D {
    Circle,
    Box
};

struct Collider2D
{
    Shape2D shape = Shape2D::Box;
    glm::vec2 offset = glm::vec2(0.0f, 0.0f);

    // Box
    glm::vec2 halfxtents = glm::vec2(1.0f, 1.0f);

    // Circle
    float radius = 1.0f;
};
