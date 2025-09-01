#include "Scene.h"
#include "components/Camera.h"
#include "core/Registry.h"
#include "core/Types.h"
#include "components/Transform.h"
#include <cassert>
#include <functional>

Scene::Scene() {}

Scene::Scene(std::string_view name, std::function<void(Registry &)> setup) : m_Name(name), m_Setup(setup) {}

EntityId Scene::GetCamera()
{
    std::vector<EntityId> entities = m_Registry.View<Camera, Transform>();
    assert(entities.size() >= 1 && "No Cameras found in scene");

    return entities[0];
}
