#pragma once

#include "components/Transform.h"
#include "components/Camera.h"
#include "core/SystemManager.h"
#include "core/Entity.h"

class CameraSystem : public System
{
   public:
    CameraSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<Camera, Transform>();
    }

    void Update() override
    {
        for (Entity e : m_Entities) {
            auto [cam, transform] = e.GetComponents<Camera, Transform>();
            if (cam.isActive) {
                Simplex::GetView().SetCamera(transform, cam);
            }
        }
    }
};
