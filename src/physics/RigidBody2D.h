#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <vector>

enum class PhysicsType {
    Static,
    Dynamic
};

struct Force2D
{
    glm::vec2 position = glm::vec2(0.0f, 0.0f);
    glm::vec2 direction = glm::vec2(0.0f, 0.0f);
    float magnitude;
};

struct RigidBody2D
{
    PhysicsType type = PhysicsType::Dynamic;
    std::vector<Force2D> forces;
    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
    float mass = 10.0f;
};
