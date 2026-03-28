#pragma once

#include "core/IView.h"

class FakeView : public IView
{
  public:
    FakeView(int width = 1280, int height = 720);
    ~FakeView();

    // Window
    GLFWwindow *GetWindow() override;
    std::pair<int &, int &> GetWindowDimensions() override;
    void SetWindowDimensions(int width, int height) override;
    int &GetWindowHeight() override;
    int &GetWindowWidth() override;
    bool HasWindowResized() override;

    // Camera
    void SetCameraBounds(Transform transform, Camera camera) override;
    RectBounds<float> GetCameraBounds() override;
    glm::mat4 CalculateProjection(RenderSpace renderSpace) override;

    // GLFW Wrappers
    bool Init(std::string_view title, int width, int height) override;
    bool ShouldQuit() override;
    void ClearColor(glm::vec4 color) override;
    void SwapBuffers() override;

    // Test helpers
    void SimulateResize(int width, int height);
    void SimulateQuit();

  private:
    int m_Width;
    int m_Height;
    bool m_HasWindowResized = false;
    bool m_ShouldQuit = false;
    RectBounds<float> m_CameraBounds;
};
