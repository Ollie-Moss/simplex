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
        m_Registry.RegisterSystem<UISystem>();

        // Entities
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(-50, 100, 0)}, {.texture = "GRASS_TILE_1"});
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(0, 100, 0)}, {.texture = "GRASS_TILE_1"});
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(50, 100, 0)}, {.texture = "GRASS_TILE_1"});
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(100, 100, 0)}, {.texture = "GRASS_TILE_1"});
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(100, 110, 0)}, {.color = BLUE});
        m_Registry.Create<Transform, Sprite>({.position = glm::vec3(50, 110, 0)}, {.color = BLUE});

        m_Registry.Create<Transform, Camera, MoveableCamera>({}, {}, {});

        auto elemOld = element(
            {
                .sizing = {.width = GROW, .height = GROW},
                .padding = 10.0_p,
            },
            {
                element(
                    {
                        .sizing = {.width = GROW, .height = 200.0_pixels},
                        .padding = 10.0_p,
                        .color = GREEN,
                        .gap = 10.0f,
                    },
                    {
                        element({.sizing = {.width = GROW, .height = GROW}, .color = RED}),
                        element({.sizing = {.width = GROW, .height = GROW}, .color = BLUE}),
                    }),
                element(
                    {
                        .sizing = {.width = 200.0_pixels, .height = 200.0_pixels},
                        .direction = FlexDirection::Column,
                        .padding = 10.0_p,
                        .color = YELLOW,
                        .gap = 10.0f,
                    },
                    {
                        element({.sizing = {.width = GROW, .height = GROW}, .color = RED}),
                        element({.sizing = {.width = GROW, .height = GROW}, .color = RED}),
                        element({.sizing = {.width = GROW, .height = GROW}, .color = BLUE}),
                    }),
            });

        auto elem = element(
            {
                .sizing = {.width = GROW, .height = GROW},
                .padding = 10.0_p,
                        .gap = 10.0f,
            },
            {
                element(
                    {
                        .sizing = {.width = GROW, .height = 200.0_pixels},
                        .padding = 10.0_p,
                        .color = GREEN,
                    }),
                element(
                    {
                        .sizing = {.width = GROW, .height = 200.0_pixels},
                        .padding = 10.0_p,
                        .color = RED,
                    }),
            });

        Entity root = CreateEntityFromUISpec(m_Registry, elem);
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
