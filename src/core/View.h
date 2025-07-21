#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float4.hpp"
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

    std::pair<int&, int&> GetWindowDimensions();
    void SetWindowDimensions(int width, int height);
    int &GetWindowHeight();
    int &GetWindowWidth();

    static void FramebufferSizeCallback(GLFWwindow *window, int newWidth, int newHeight);

    operator GLFWwindow *() {
        return m_Window;
    }

  private:
    GLFWwindow *m_Window;
    int m_Width;
    int m_Height;
    std::string m_Title;
};
