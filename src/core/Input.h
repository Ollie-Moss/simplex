#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <functional>
#include "glm/glm.hpp"
#include "graphics/util/RenderSpace.h"
#include <map>
#include <string>
#include <vector>

enum class KeyState {
    Released,
    FirstPress,
    Pressed,
};

class Input
{
  public:
    Input();
    ~Input();

    bool Init();

    bool OnKeyPressed(int button);
    bool OnMouseButtonPressed(int button);

    bool OnKeyDown(int button);
    bool OnMouseButtonDown(int button);

    std::string GetTextInput();

    glm::vec2 GetMousePosition(RenderSpace space = RenderSpace::Screen);
    glm::vec2 GetMouseDelta();

    float GetScrollDelta();

    void PollEvents();

    void SetMouseButtonState(int button, KeyState state);
    void SetKeyState(int button, KeyState state);

  private:
    void ResetMouseButtons();
    void ResetKeys();

    static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void CharacterCallback(GLFWwindow *window, unsigned int codepoint);

  private:
    std::map<int, KeyState> m_MouseButtonState;
    std::map<int, KeyState> m_KeyState;

    glm::vec2 m_CurrentMousePosition;
    glm::vec2 m_LastMousePosition;
    float m_Scroll;

    std::vector<std::function<void(float)>> m_ScrollCallbacks;

    std::map<int, bool> m_MouseButtonStateConsumed;
    std::map<int, bool> m_KeyStateConsumed;

    std::string m_TextInputBuffer;
};
