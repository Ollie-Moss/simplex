#pragma once

#include "core/Registry.h"
#include "core/SimplexModules.h"
#include "core/Types.h"
#include <functional>

class Scene
{
  public:
    Scene(const SimplexModules &modules, std::string_view name, std::function<void(Registry &)> setup);
    Scene() = default;
    ~Scene() = default;

    Scene(const Scene &) = default;
    Scene(Scene &&) = default;

    Scene &operator=(const Scene &) = default;
    Scene &operator=(Scene &&) = default;

    EntityId GetCamera();

    std::function<void(Registry &)> m_Setup = [](Registry &) {};
    std::string m_Name = "";
    Registry m_Registry;
};
