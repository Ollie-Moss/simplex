#pragma once

#include "components/Transform.h"
#include "components/Camera.h"
#include "components/MoveableCamera.h"
#include "core/Registry.h"
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
    MoveableCameraSystem(Registry &registry, const SimplexModules &modules) : System(registry, modules)
    {
        m_Signature = m_Registry.CreateSignature<MoveableCamera, Camera, Transform>();
    }

    void Update(float timeStep) override
    {
        for(EntityId e : m_Entities)
        {
            auto &moveableCam = m_Registry.GetComponent<MoveableCamera &>(e);
            auto &cam = m_Registry.GetComponent<Camera &>(e);
            auto &transform = m_Registry.GetComponent<Transform &>(e);

            glm::vec2 mouseDelta = m_Modules.m_Input->GetMouseDelta();
            float scrollDelta = m_Modules.m_Input->GetScrollDelta();

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

            if(m_Modules.m_Input->GetMouseButton(GLFW_MOUSE_BUTTON_1).IsDown())
            {
                auto targetTransform = transform.position + glm::vec3(mouseDelta / cam.zoom, 0.0f);
                transform.position = targetTransform;
            }
        }
    }
};
