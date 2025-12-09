#include "components/Sprite.h"
#include "components/Transform.h"
#include "core/Entity.h"
#include "core/Scene.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "physics/DebugPhysicsSystem.h"
#include "gui/UIBuilder.h"
#include "gui/UIComponents.h"
#include "gui/UISystem.h"
#include "physics/RigidBody2D.h"
#include "systems/CameraSystem.h"
#include "systems/MoveableCameraSystem.h"
#include "systems/RenderSystem.h"
#include "physics/IntegrationSystem.h"
#include <optional>

struct Movement
{
    float moveSpeed = 50.0f;
};

struct PlayerStats
{
    float health = 100.0f;
};

class MovementSystem : public System
{
  public:
    MovementSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<Transform, Movement>();
    }
    void Update(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            auto &stats = e.GetComponent<PlayerStats>();

            auto [transform, movement, rb] = e.GetComponents<Transform, Movement, RigidBody2D>();
            if(Simplex::GetInput().OnKeyPressed(GLFW_KEY_R))
            {
                transform.position = glm::vec3(0, 0, 0);
                rb.velocity.y = 0;
                stats.health -= 1;
            }

            int horizontalAxis = Simplex::GetInput().OnKeyDown(GLFW_KEY_A) * -1 + Simplex::GetInput().OnKeyDown(GLFW_KEY_D);
            int verticalAxis = Simplex::GetInput().OnKeyDown(GLFW_KEY_S) * -1 + Simplex::GetInput().OnKeyDown(GLFW_KEY_W);

            transform.position.x += horizontalAxis * movement.moveSpeed * timeStep;
            transform.position.y += verticalAxis * movement.moveSpeed * timeStep;
        }
    }
};

UISpec SideBar(Entity player)
{
    return element(
        {
            .layout = UILayout{
                .sizing = Sizing{
                    .width = Axis(SizingMode::Fixed, Percent(20.0f)),
                    .height = Axis(SizingMode::Fixed, Pixels(100.0f))},
                .direction = Direction::Vertical,
                .padding = 10.0,
                .gap = 10.0f,
            },
        },
        {
            element({
                .layout = UILayout{
                    .sizing = Sizing{
                        .width = Bind<Axis>(player, [&](std::optional<Entity> player) {
                            auto stats = player->GetComponent<PlayerStats>();

                            return Axis(SizingMode::Fixed, Percent(stats.health));
                        }),
                        .height = HUG,
                    },
                    .direction = Direction::Vertical,
                    .gap = 10.0f,
                },
                .style = UIStyle{.color = GREEN},
                .text = Text{
                    .content = Bind<std::string>(player, [](std::optional<Entity> target) {
                        return std::to_string((*target).GetComponent<PlayerStats>().health);
                    }),
                },
            }),

        });
}

int main()
{
    Simplex simplex;
    if(!simplex.Init())
    {
        return 0;
    }
    Scene MainScene = Scene("MainScene", [](Registry &m_Registry) {
        // Systems
        m_Registry.RegisterSystem<CameraSystem>();
        m_Registry.RegisterSystem<MoveableCameraSystem>();

        // Physics
        m_Registry.RegisterSystem<IntegrationSystem>();

        // UI
        m_Registry.RegisterSystem<UIStateSystem>();
        m_Registry.RegisterSystem<UILayoutSystem>();

        // Custom
        m_Registry.RegisterSystem<MovementSystem>();

        // Rendering
        m_Registry.RegisterSystem<UIRenderSystem>();
        m_Registry.RegisterSystem<RenderSystem>();
        m_Registry.RegisterSystem<DebugPhysicsSystem>();

        // Entities
        m_Registry.Create<Transform, Sprite, Collider2D>({.position = glm::vec3(-2, -3, 0), .size = glm::vec2(5, 1)}, {.color = TRANSPARENT}, {.shape = Shape2D::Box});

        Entity player = m_Registry.Create<Transform, Sprite, Movement, Collider2D, RigidBody2D, PlayerStats>(
            {},
            {.color = BLUE},
            {},
            {},
            {},
            {} //
        );

        m_Registry.Create<Transform, Camera, MoveableCamera>({}, {}, {});

        Entity root = BuildUI(m_Registry, SideBar(player));
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
