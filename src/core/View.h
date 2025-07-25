#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "core/RectBounds.h"
#include "glm/glm.hpp"
#include <string_view>
#include <string>

class View {
  public:
    View();
    ~View();

    bool Init(std::string_view title, int width, int height);
    bool ShouldQuit();
    void ClearColor(glm::vec4 color);
    void SwapBuffers();

    GLFWwindow *GetWindow();

    std::pair<int &, int &> GetWindowDimensions();
    void SetWindowDimensions(int width, int height);
    int &GetWindowHeight();
    int &GetWindowWidth();

    static void FramebufferSizeCallback(GLFWwindow *window, int newWidth, int newHeight);

    RectBounds<float> GetCameraBounds();
    glm::mat4 CalculateWorldSpaceProjection();
    glm::mat4 CalcualteScreenSpaceProjection();

    operator GLFWwindow *() {
        return m_Window;
    }

  private:
    GLFWwindow *m_Window;
    int m_Width;
    int m_Height;
    std::string m_Title;
};
