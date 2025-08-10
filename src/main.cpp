#include <vector>
#include "core/Scene.h"
#include "core/Simplex.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include "gui/UIComponents.h"
#include "gui/UISystem.h"
#include "systems/CameraSystem.h"
#include "systems/MoveableCameraSystem.h"
#include "systems/RenderSystem.h"

int main()
{
    Simplex simplex;
    if (!simplex.Init()) {
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

        // start element
        Entity parent = m_Registry.Create<UITransform, UIElement, UIProperties>({.position = glm::vec2(100, 100)}, {}, {.color = RED});
        Entity child = m_Registry.Create<UITransform, UIElement, UIProperties>({.size = glm::vec2(20, 20)}, {.parent = parent}, {.color = BLUE});
        Entity child2 = m_Registry.Create<UITransform, UIElement, UIProperties>({.size = glm::vec2(20, 20)}, {.parent = parent}, {.color = BLUE});
        // close element
        parent.GetComponent<UIElement>().children = {child, child2};
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
