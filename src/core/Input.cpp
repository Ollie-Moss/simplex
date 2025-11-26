#include "Input.h"
#include "core/Simplex.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <string>
#include <sys/types.h>

Input::Input() {}
Input::~Input() {}

bool Input::Init()
{
    glfwSetMouseButtonCallback(Simplex::GetView().GetWindow(), MouseButtonCallback);
    glfwSetKeyCallback(Simplex::GetView().GetWindow(), KeyCallback);
    glfwSetScrollCallback(Simplex::GetView().GetWindow(), ScrollCallback);

    return true;
}

void Input::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    switch(action)
    {
    case GLFW_PRESS:
        Simplex::GetInput().SetMouseButtonState(button, KeyState::FirstPress);
        break;
    case GLFW_RELEASE:
        Simplex::GetInput().SetMouseButtonState(button, KeyState::Released);
        break;
    default:
        break;
    }
}
void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    switch(action)
    {
    case GLFW_PRESS:
        Simplex::GetInput().SetKeyState(key, KeyState::FirstPress);
        break;
    case GLFW_RELEASE:
        Simplex::GetInput().SetKeyState(key, KeyState::Released);
        break;
    default:
        break;
    }
}

void Input::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    Simplex::GetInput().m_Scroll = static_cast<float>(yoffset);
}

void Input::SetMouseButtonState(int button, KeyState state)
{
    m_MouseButtonState[button] = state;
}

void Input::SetKeyState(int button, KeyState state)
{
    m_KeyState[button] = state;
}

bool Input::OnKeyDown(int key)
{
    if(Simplex::GetView().GetWindow() == nullptr)
    {
        return false;
    }

    auto state = m_KeyState[key];
    return state == KeyState::FirstPress || state == KeyState::Pressed;
}

bool Input::OnMouseButtonDown(int button)
{
    if(Simplex::GetView().GetWindow() == nullptr)
    {
        return false;
    }

    auto state = m_MouseButtonState[button];
    return state == KeyState::FirstPress || state == KeyState::Pressed;
}

bool Input::OnMouseButtonPressed(int button)
{
    if(Simplex::GetView().GetWindow() == nullptr)
    {
        return false;
    }

    KeyState &state = m_MouseButtonState[button];
    bool isPressed = state == KeyState::FirstPress;
    if(isPressed)
        m_KeyStateConsumed[button] = true;

    return isPressed;
}
bool Input::OnKeyPressed(int button)
{
    if(Simplex::GetView().GetWindow() == nullptr)
    {
        return false;
    }

    KeyState &state = m_KeyState[button];
    bool isPressed = state == KeyState::FirstPress;

    if(isPressed)
        m_KeyStateConsumed[button] = true;

    return isPressed;
}

glm::vec2 Input::GetMouseDelta()
{
    return m_LastMousePosition - m_CurrentMousePosition;
}

float Input::GetScrollDelta()
{
    return m_Scroll;
}

glm::vec2 Input::GetMousePosition()
{
    double mouseX, mouseY;
    glfwGetCursorPos(Simplex::GetView(), &mouseX, &mouseY);
    glm::vec2 mousePos = glm::vec2((float)mouseX, Simplex::GetView().GetWindowHeight() - (float)mouseY);
    return mousePos;
}

void Input::PollEvents()
{
    ResetMouseButtons();
    ResetKeys();

    m_Scroll = 0;
    m_LastMousePosition = m_CurrentMousePosition;
    m_CurrentMousePosition = GetMousePosition();

    glfwPollEvents();
}

void Input::ResetMouseButtons()
{
    for(auto &[button, state] : m_MouseButtonState)
    {
        if(state == KeyState::FirstPress && m_MouseButtonStateConsumed[button])
        {
            state = KeyState::Pressed;
            m_MouseButtonStateConsumed[button] = false;
        }
    }
}
void Input::ResetKeys()
{
    for(auto &[key, state] : m_KeyState)
    {
        if(state == KeyState::FirstPress && m_KeyStateConsumed[key])
        {
            state = KeyState::Pressed;
            m_KeyStateConsumed[key] = false;
        }
    }
}
