#pragma once

#include "components/Transform.h"
#include "components/Camera.h"
#include "components/MoveableCamera.h"
#include "core/SystemManager.h"
#include "core/Entity.h"

class MoveableCameraSystem : public System
{
   public:
    MoveableCameraSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<MoveableCamera, Camera, Transform>();
    }

    void Update() override
    {
        for (Entity e : m_Entities) {
            auto [moveableCam, cam, transform] = e.GetComponents<MoveableCamera, Camera, Transform>();
            glm::vec2 mouseDelta = Simplex::GetInput().GetMouseDelta();
            float scrollDelta = Simplex::GetInput().GetScrollDelta();

            cam.zoom += scrollDelta * (moveableCam.scrollSensitivity * cam.zoom / 10.0f);
            if(cam.zoom > moveableCam.maxZoom)
                cam.zoom = moveableCam.maxZoom;
            if(cam.zoom < moveableCam.minZoom)
                cam.zoom = moveableCam.minZoom;

            if (Simplex::GetInput().OnMouseButtonDown(GLFW_MOUSE_BUTTON_1)) {
                transform.position += glm::vec3(mouseDelta / cam.zoom, 0.0f);
            }
        }
    }
};
