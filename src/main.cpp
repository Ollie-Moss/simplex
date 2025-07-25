#include "core/Scene.h"
#include "core/Simplex.h"
#include "glm/fwd.hpp"
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
        m_Registry.RegisterSystem<RenderSystem>();
        m_Registry.RegisterSystem<MoveableCameraSystem>();
        m_Registry.RegisterSystem<CameraSystem>();

        // Entities
        m_Registry.Create<Transform, SpriteRenderer>({.position = glm::vec3(100, 100, 0)}, {.texture = "GRASS_TILE_1"});
        m_Registry.Create<Transform, Camera, MoveableCamera>({}, {}, {});
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
