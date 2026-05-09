#pragma once

#include "components/Transform.h"
#include "components/Camera.h"
#include "core/Registry.h"
#include "core/Types.h"

class CameraSystem : public System
{
  public:
    CameraSystem(Registry &registry, const SimplexModules &modules) : System(registry, modules)
    {
        m_Signature = m_Registry.CreateSignature<Camera, Transform>();
    }

    void Update(float timeStep) override
    {
        for(EntityId e : m_Entities)
        {
            auto &cam = m_Registry.GetComponent<Camera &>(e);
            auto &transform = m_Registry.GetComponent<Transform &>(e);

            if(cam.isActive)
            {
                m_Modules.m_View->SetCameraBounds(transform, cam);
            }
        }
    }
};
