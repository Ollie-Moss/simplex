#include "core/Registry.h"
#include "core/Scene.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "glm/glm.hpp"
#include "gui/UIBuilder.h"
#include "gui/UIComponents.h"
#include "gui/UISystem.h"
#include "systems/CameraSystem.h"
#include "systems/MoveableCameraSystem.h"
#include "systems/RenderSystem.h"
#include <initializer_list>

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

        m_Registry.RegisterSystem<UILayoutSystem>();
        m_Registry.RegisterSystem<UIRenderSystem>();

        // Entities
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(-50, 100, 0)},
                                             {.texture = "GRASS_TILE_1"});
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(0, 100, 0)},
                                             {.texture = "GRASS_TILE_1"});
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(50, 100, 0)},
                                             {.texture = "GRASS_TILE_1"});

        m_Registry.Create<Transform, Camera, MoveableCamera>({}, {}, {});

        auto elem = element(
            {
                .layout = UILayout{
                    .sizing = {.width = GROW, .height = GROW},
                    .direction = Direction::Vertical,
                    .padding = 10.0_p,
                    .gap = 10.0f,
                },
            },
            {
                element({
                            .layout = UILayout{
                                .sizing = {.width = GROW, .height = GROW},
                                .direction = Direction::Horizontal,
                                .padding = 10.0_p,
                                .gap = 20.0f,
                                .alignItems = AlignItems::Start,
                                .justifyContent = JustifyContent::Start,
                            },
                            .style = UIStyle{

                                .color = YELLOW,
                            },
                        },
                        {
                            element({
                                .layout = UILayout{
                                    .sizing = {.width = 50.0_pixels, .height = 50.0_pixels},
                                    .padding = 10.0_p,
                                },
                                .style = UIStyle{

                                    .color = GREEN,
                                },
                            }),
                            element({
                                .layout = UILayout{
                                    .sizing = {.width = 20.0_percent, .height = HUG},
                                    .padding = 10.0_p,
                                },
                                .style = UIStyle{

                                    .color = RED,
                                },
                                .text = UIText{
                                    .text = {.content = "Hello World! abc", .fontSize = 10},
                                },
                            }),
                        }),
                element({
                            .layout = UILayout{
                                .sizing{.width = GROW, .height = GROW},
                            },
                        },
                        {
                            element({
                                .layout = UILayout{
                                    .sizing = {.width = 20.0_percent, .height = GROW},
                                    .padding = 10.0_p,
                                },
                                .style = UIStyle{
                                    .color = GREEN,
                                },
                            }),
                            element({
                                .layout = UILayout{
                                    .sizing = {.width = GROW, .height = GROW},
                                    .padding = 10.0_p,
                                },
                                .style = UIStyle{
                                    .color = RED,
                                },
                            }),
                        }),
            });

        Entity root = CreateEntityFromUISpec(m_Registry, elem);
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
