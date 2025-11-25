#pragma once

#include "components/Transform.h"
#include "components/Camera.h"
#include "components/MoveableCamera.h"
#include "core/SystemManager.h"
#include "core/Entity.h"
#include <algorithm>
#include <cmath>

inline double easeOut(double t)
{
    double t2 = (--t) * t;
    return 1 + t * t2 * t2;
}

template <typename T>
T lerp(const T &a, const T &b, float t)
{
    return a + (b - a) * t;
}

class MoveableCameraSystem : public System
{
    float t = 0;

  public:
    MoveableCameraSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<MoveableCamera, Camera, Transform>();
    }

    void Update(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            auto [moveableCam, cam, transform] = e.GetComponents<MoveableCamera, Camera, Transform>();
            glm::vec2 mouseDelta = Simplex::GetInput().GetMouseDelta();
            float scrollDelta = Simplex::GetInput().GetScrollDelta();

            float zoomDelta = scrollDelta * (moveableCam.scrollSensitivity * cam.zoom / 10.0f);
            float targetZoom = cam.targetZoom + zoomDelta;
            targetZoom = std::clamp(targetZoom, moveableCam.minZoom, moveableCam.maxZoom);

            if(scrollDelta != 0.0f)
            {
                cam.startTargetZoom = cam.zoom;
                cam.targetZoom = targetZoom;
                t = 0.0f;
            }

            float duration = 0.2f;
            t += timeStep / duration;
            t = std::clamp(t, 0.0f, 1.0f);

            float eased = easeOut(t);
            cam.zoom = lerp(cam.startTargetZoom, cam.targetZoom, eased);

            if(Simplex::GetInput().OnMouseButtonDown(GLFW_MOUSE_BUTTON_1))
            {
                auto targetTransform = transform.position + glm::vec3(mouseDelta / cam.zoom, 0.0f);
                transform.position = targetTransform;
            }
        }
    }
};
