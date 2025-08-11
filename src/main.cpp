#include "core/Registry.h"
#include "core/Scene.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "glm/glm.hpp"
#include "gui/UIBuilder.h"
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
        auto elem = element(
          {
            .sizing = {.width = 100.0_percent, .height = 100.0_percent},
            .padding = 10.0_p,
            .color = GREEN,
            .gap = 10.0f,
          },
          {
            element({.sizing = {.width = 50.0_pixels, .height = 50.0_pixels}, .color = RED}),
            element({.sizing = {.height = 50.0_pixels}, .padding = 5.0_p, .color = BLUE},
                    {
                      element({.sizing = {.width = 25.0_pixels, .height = 10.0_pixels}, .color = YELLOW}),
                    }),
          });

        Entity root = CreateEntityFromUISpec(m_Registry, elem);
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
