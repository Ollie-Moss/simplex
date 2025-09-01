#pragma once

#include "components/Camera.h"
#include "components/Transform.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "core/RectBounds.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <string_view>
#include <string>

class View
{
  public:
    View();
    ~View();

    // Window
    GLFWwindow *GetWindow();
    std::pair<int &, int &> GetWindowDimensions();
    void SetWindowDimensions(int width, int height);
    int &GetWindowHeight();
    int &GetWindowWidth();

    bool HasWindowResized();

    // Camera
    void SetCamera(Transform transform, Camera camera);
    RectBounds<float> GetCameraBounds();

    glm::mat4 CalculateWorldSpaceProjection();
    glm::mat4 CalculateScreenSpaceProjection();

    // GLFW Wrappers
    bool Init(std::string_view title, int width, int height);
    bool ShouldQuit();
    void ClearColor(glm::vec4 color);
    void SwapBuffers();

    static void FramebufferSizeCallback(GLFWwindow *window, int newWidth, int newHeight);

    operator GLFWwindow *()
    {
        return m_Window;
    }

  private:
    GLFWwindow *m_Window;
    int m_Width;
    int m_Height;
    std::string m_Title;

    bool m_HasWindowResized = true;

    RectBounds<float> m_CameraBounds;
};
