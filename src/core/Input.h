#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <functional>
#include "glm/glm.hpp"
#include <map>
#include <vector>

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

    glm::vec2 GetMousePosition();
    glm::vec2 GetMouseDelta();

    float GetScrollDelta();

    void PollEvents();

    void SetMouseButtonState(int button, bool state);
    void SetKeyState(int button, bool state);

   private:
    void ResetMouseButtons();
    void ResetKeys();

    static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

   private:
    std::map<int, bool> m_MouseButtonState;
    std::map<int, bool> m_KeyState;

    glm::vec2 m_CurrentMousePosition;
    glm::vec2 m_LastMousePosition;
    float m_Scroll;

    std::vector<std::function<void(float)>> m_ScrollCallbacks;
};
