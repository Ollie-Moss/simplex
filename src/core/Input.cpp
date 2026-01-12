#include "core/Simplex.h"
#include "Input.h"
#include "glm/fwd.hpp"
#include "graphics/util/RenderSpace.h"
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
    glfwSetCharCallback(Simplex::GetView().GetWindow(), CharacterCallback);

    return true;
}

void Input::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    Simplex::GetInput().SetMouseButton(button, Key((KeyState)action, (KeyMod)mods));

    if(Simplex::GetInput().GetMouseButton(button).IsDown())
        Simplex::GetInput().m_MouseInputBuffer.push_back(button);
}

void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Simplex::GetInput().SetKey(key, Key((KeyState)action, (KeyMod)mods));

    if(key == GLFW_KEY_ENTER && (action == GLFW_PRESS || action == GLFW_REPEAT))
        Simplex::GetInput().m_TextInputBuffer += "\n";
    if(key == GLFW_KEY_TAB && (action == GLFW_PRESS || action == GLFW_REPEAT))
        Simplex::GetInput().m_TextInputBuffer += "\t";
    if(key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
        Simplex::GetInput().m_TextInputBuffer += "\b";
}

void Input::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    Simplex::GetInput().m_Scroll = static_cast<float>(yoffset);
}

void Input::CharacterCallback(GLFWwindow *window, unsigned int codepoint)
{
    Simplex::GetInput().m_TextInputBuffer += (unsigned char)codepoint;
}

//---------------------------=
// Keyboard Input
//---------------------------=
std::string Input::GetTextInput()
{
    return m_TextInputBuffer;
}

void Input::SetKey(int glfwButton, const Key &key)
{
    m_Keys[glfwButton] = key;
}

Key &Input::GetKey(int glfwButton)
{
    return m_Keys[glfwButton];
}

//---------------------------=
// Mouse Input
//---------------------------=

const std::vector<int> &Input::GetMouseInput()
{
    return m_MouseInputBuffer;
}

void Input::SetMouseButton(int glfwButton, const Key &key)
{
    m_MouseButtons[glfwButton] = key;
}

Key &Input::GetMouseButton(int glfwButton)
{
    return m_MouseButtons[glfwButton];
};

float Input::GetScrollDelta()
{
    return m_Scroll;
}

//---------------------------=
// Mouse Position
//---------------------------=

glm::vec2 Input::GetMouseDelta()
{
    return m_LastMousePosition - m_CurrentMousePosition;
}

glm::vec2 Input::GetMousePosition(RenderSpace space)
{
    double mouseX, mouseY;
    glfwGetCursorPos(Simplex::GetView(), &mouseX, &mouseY);
    // glm::vec2 mousePos = glm::vec2((float)mouseX, Simplex::GetView().GetWindowHeight() - (float)mouseY);

    if(space == RenderSpace::World)
        mouseY = Simplex::GetView().GetWindowHeight() - (float)mouseY;

    glm::vec2 mousePos = {mouseX, mouseY};

    return mousePos;
}

//---------------------------=
// Helpers
//---------------------------=

void Input::PollEvents()
{
    for(auto &[glfwButton, key] : m_Keys)
        key.Reset();

    for(auto &[glfwButton, key] : m_MouseButtons)
        key.Reset();

    m_Scroll = 0;
    m_LastMousePosition = m_CurrentMousePosition;
    m_CurrentMousePosition = GetMousePosition();
    m_TextInputBuffer.clear();
    m_MouseInputBuffer.clear();

    glfwPollEvents();
}
