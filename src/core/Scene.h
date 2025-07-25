#pragma once

#include "core/Registry.h"
#include "core/Types.h"
#include <functional>

class Scene
{
   public:
    Scene();
    Scene(std::string_view name, std::function<void(Registry &)> setup);

    EntityId GetCamera();

    std::function<void(Registry &)> m_Setup;
    std::string m_Name;
    Registry m_Registry;
};
