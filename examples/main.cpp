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
#include <format>
#include <string>

struct Movement
{
    float moveSpeed = 50.0f;
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
            auto [transform, movement, rb] = e.GetComponents<Transform, Movement, RigidBody2D>();
            if(Simplex::GetInput().OnKeyPressed(GLFW_KEY_R))
            {
                transform.position = glm::vec3(0, 0, 0);
                rb.velocity.y = 0;
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
                .sizing = {.width = 20.0_percent, .height = 100.0_percent},
                .direction = Direction::Vertical,
                .padding = 10.0_p,
                .gap = 10.0f,
            },
        },
        {

            element({
                        .layout = UILayout{
                            .sizing = {.width = GROW, .height = 50.0_percent},
                            .direction = Direction::Horizontal,
                        },
                        .style = UIStyle{.color = GREEN},
                    },
                    {
                        element({
                            .layout = UILayout{
                                .sizing = {.width = GROW, .height = HUG},
                            },
                            .style = UIStyle{.color = TRANSPARENT},
                            .bindText = Bind<std::string>(std::nullopt, [&](std::optional<Entity> target) {
                                auto fps = Simplex::Get().GetFPS();
                                return std::format("fps: {:.0f}", fps);
                            }),
                        }),
                        element({
                            .layout = UILayout{
                                .sizing = {.width = GROW, .height = HUG},
                            },
                            .style = UIStyle{.color = TRANSPARENT},
                            .bindText = Bind<std::string>(player, [&](std::optional<Entity> target) {
                                auto &pos = target.value().GetComponent<Transform>().position;
                                return std::format("x: {:.0f} \ny: {:.0f} ", pos.x, pos.y);
                            }),
                        }),
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

        Entity player = m_Registry.Create<Transform, Sprite, Movement, Collider2D, RigidBody2D>(
            {},
            {.color = BLUE},
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
