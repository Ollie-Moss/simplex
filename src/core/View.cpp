#include "View.h"
#include "Simplex.h"
#include "components/Camera.h"
#include "core/Entity.h"
#include "core/Scene.h"
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include "components/Transform.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string_view>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

View::View() {}
View::~View()
{
    glfwTerminate();
}

bool View::Init(std::string_view title, int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_Title = title;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

    if (m_Window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, m_Width, m_Height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetFramebufferSizeCallback(m_Window, View::FramebufferSizeCallback);

    return true;
}

GLFWwindow *View::GetWindow()
{
    return m_Window;
}

std::pair<int &, int &> View::GetWindowDimensions()
{
    return {m_Width, m_Height};
}

void View::SetWindowDimensions(int width, int height)
{
    m_Width = width;
    m_Height = height;
}

int &View::GetWindowWidth()
{
    return m_Width;
}

int &View::GetWindowHeight()
{
    return m_Height;
}

void View::FramebufferSizeCallback(GLFWwindow *window, int newWidth, int newHeight)
{
    Simplex::GetView().SetWindowDimensions(newWidth, newHeight);
    auto [width, height] = Simplex::GetView().GetWindowDimensions();
    glViewport(0, 0, width, height);
}

void View::ClearColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}
void View::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

bool View::ShouldQuit()
{
    return glfwWindowShouldClose(m_Window);
}

void View::SetCamera(Transform transform, Camera camera){

    float orthoWidth = m_Width / camera.zoom;
    float orthoHeight = m_Height / camera.zoom;

    float cameraLeft = transform.position.x - orthoWidth / 2.0f;
    float cameraRight = transform.position.x + orthoWidth / 2.0f;
    float cameraTop = transform.position.y - orthoHeight / 2.0f;
    float cameraBottom = transform.position.y + orthoHeight / 2.0f;

    m_CameraBounds = {.top = cameraTop, .right = cameraRight, .bottom = cameraBottom, .left = cameraLeft};
}

RectBounds<float> View::GetCameraBounds()
{
    return m_CameraBounds;
}

glm::mat4 View::CalculateWorldSpaceProjection()
{
    auto bounds = GetCameraBounds();
    float nearZClip = -100.0f;
    float farZClip = 100.0f;

    glm::mat4 projection = glm::ortho(bounds.left, bounds.right, bounds.top, bounds.bottom, nearZClip, farZClip);
    return projection;
}

glm::mat4 View::CalculateScreenSpaceProjection()
{
    glm::mat4 projection = glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -100.0f, 100.0f);
    return projection;
}
