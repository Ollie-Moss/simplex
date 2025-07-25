#pragma once
struct Camera {
    float zoom = 1.0f;
    float maxZoom = 3.0f;
    float scrollSensitivity = 0.5f;

    bool mainCamera = false;
};
