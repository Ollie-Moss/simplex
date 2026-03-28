#include "FakeView.h"

FakeView::FakeView(int width, int height)
    : m_Width(width), m_Height(height) {}

FakeView::~FakeView() {}

// Window
GLFWwindow *FakeView::GetWindow() { return nullptr; }

std::pair<int &, int &> FakeView::GetWindowDimensions() { return {m_Width, m_Height}; }

void FakeView::SetWindowDimensions(int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_HasWindowResized = true;
}

int &FakeView::GetWindowWidth() { return m_Width; }
int &FakeView::GetWindowHeight() { return m_Height; }
bool FakeView::HasWindowResized() { return m_HasWindowResized; }

// Camera
void FakeView::SetCameraBounds(Transform transform, Camera camera)
{
    // no-op, or compute bounds if needed
}

RectBounds<float> FakeView::GetCameraBounds() { return m_CameraBounds; }

glm::mat4 FakeView::CalculateProjection(RenderSpace renderSpace)
{
    return glm::mat4(1.0f); // identity
}

// GLFW Wrappers
bool FakeView::Init(std::string_view title, int width, int height)
{
    m_Width = width;
    m_Height = height;
    return true;
}

bool FakeView::ShouldQuit() { return m_ShouldQuit; }
void FakeView::ClearColor(glm::vec4 color) {}
void FakeView::SwapBuffers() {}

// Test helpers
void FakeView::SimulateResize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_HasWindowResized = true;
}

void FakeView::SimulateQuit() { m_ShouldQuit = true; }
