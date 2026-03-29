#pragma once

#include "components/Camera.h"
#include "components/Transform.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "core/RectBounds.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "graphics/util/RenderSpace.h"
#include <string_view>

class IView
{
  public:
    // Window
    virtual GLFWwindow *GetWindow() = 0;
    virtual std::pair<int &, int &> GetWindowDimensions() = 0;
    virtual void SetWindowDimensions(int width, int height) = 0;
    virtual int &GetWindowHeight() = 0;
    virtual int &GetWindowWidth() = 0;

    virtual bool HasWindowResized() = 0;

    // Camera
    virtual void SetCameraBounds(Transform transform, Camera camera) = 0;
    virtual RectBounds<float> GetCameraBounds() = 0;

    virtual glm::mat4 CalculateProjection(RenderSpace renderSpace) = 0;

    // GLFW Wrappers
    virtual bool Init(std::string_view title, int width, int height) = 0;
    virtual bool ShouldQuit() = 0;
    virtual void ClearColor(glm::vec4 color) = 0;
    virtual void SwapBuffers() = 0;
};
