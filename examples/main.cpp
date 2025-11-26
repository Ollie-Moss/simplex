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
            auto [transform, movement] = e.GetComponents<Transform, Movement>();
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
                            .direction = Direction::Vertical,
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
        m_Registry.RegisterSystem<RenderSystem>();

        m_Registry.RegisterSystem<UIStateSystem>();
        m_Registry.RegisterSystem<UILayoutSystem>();
        m_Registry.RegisterSystem<UIRenderSystem>();

        m_Registry.RegisterSystem<MovementSystem>();
        m_Registry.RegisterSystem<IntegrationSystem>();
        m_Registry.RegisterSystem<DebugPhysicsSystem>();

        // Entities
        auto test = m_Registry.Create<Transform, RigidBody2D, Sprite, Collider2D>({}, {}, {.color = TRANSPARENT}, {.shape = Shape2D::Box});

        m_Registry.Create<Transform, Sprite>(
            {},
            {.texture = "GRASS_TILE_1"});

        m_Registry.Create<Transform, Sprite>(
            {},
            {.texture = "GRASS_TILE_1"});

        m_Registry.Create<Transform, Sprite>(
            {},
            {.texture = "GRASS_TILE_1"});

        Entity player = m_Registry.Create<Transform, Sprite, Movement>(
            {},
            {.color = BLUE},
            {});

        m_Registry.Create<Transform, Camera, MoveableCamera>({}, {}, {});

        Entity root = BuildUI(m_Registry, SideBar(test));
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
