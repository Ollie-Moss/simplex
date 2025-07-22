#include "Input.h"
#include "core/Simplex.h"
#include <GLFW/glfw3.h>
#include <cassert>

Input::Input() {}
Input::~Input() {}

bool Input::Init(){
    glfwSetMouseButtonCallback(Simplex::GetView().GetWindow(), MouseButtonCallback);
    glfwSetKeyCallback(Simplex::GetView().GetWindow(), KeyCallback);
    glfwSetScrollCallback(Simplex::GetView().GetWindow(), ScrollCallback);

    return true;
}

void Input::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    assert(action == 1 || action == 0);
    bool pressed = static_cast<bool>(action);
    Simplex::GetInput().SetMouseButtonState(button, pressed);
}
void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    assert(action == 1 || action == 0);
    bool pressed = static_cast<bool>(action);
    Simplex::GetInput().SetKeyState(key, pressed);
}

void Input::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    Simplex::GetInput().CallScrollCallbacks(yoffset);
}

void Input::CallScrollCallbacks(float yOffset) {
    for (auto &callback : m_ScrollCallbacks) {
        callback(yOffset);
    }
}

void Input::SetMouseButtonState(int button, bool state) {
    m_MouseButtonState[button] = state;
}

void Input::SetKeyState(int button, bool state) {
    m_KeyState[button] = state;
}

bool Input::OnKeyDown(int key) {
    if (Simplex::GetView().GetWindow() == nullptr) {
        return false;
    }

    return glfwGetKey(Simplex::GetView().GetWindow(), key) == GLFW_PRESS;
}

bool Input::OnMouseButtonDown(int button) {
    if (Simplex::GetView().GetWindow() == nullptr) {
        return false;
    }

    return glfwGetMouseButton(Simplex::GetView().GetWindow(), button) == GLFW_PRESS;
}

bool Input::OnMouseButtonPressed(int button) {
    if (Simplex::GetView().GetWindow() == nullptr) {
        return false;
    }

    return m_MouseButtonState[button];
}
bool Input::OnKeyPressed(int button) {
    if (Simplex::GetView().GetWindow() == nullptr) {
        return false;
    }

    return m_KeyState[button];
}

glm::vec2 Input::GetMousePosition() {
    double mouseX, mouseY;
    glfwGetCursorPos(Simplex::GetView(), &mouseX, &mouseY);
    glm::vec2 mousePos = glm::vec2((float)mouseX, Simplex::GetView().GetWindowHeight() - (float)mouseY);
    return mousePos;
}

void Input::PollEvents() {
    ResetMouseButtons();
    ResetKeys();
    glfwPollEvents();
}

void Input::ResetMouseButtons() {
    for (auto &[button, pressed] : m_MouseButtonState) {
        pressed = false;
    }
}
void Input::ResetKeys() {
    for (auto &[key, pressed] : m_KeyState) {
        pressed = false;
    }
}
