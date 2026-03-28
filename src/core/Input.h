#pragma once

#include "GLFW/glfw3.h"
#include "core/IInput.h"
#include "core/Key.h"
#include "glm/glm.hpp"
#include <functional>
#include "glm/fwd.hpp"
#include "graphics/util/RenderSpace.h"
#include <map>
#include <string>
#include <vector>

class Input : public IInput
{
  public:
    Input();
    ~Input();

    bool Init(GLFWwindow *window) override;

    Key &GetKey(int glfwButton) override;
    Key &GetMouseButton(int glfwButton) override;

    // Returns a string of all text input received in the last frame
    std::string GetTextInput() override;

    // Returns a list of all mouse buttons pressed in the last frame in chronological order
    const std::vector<int> &GetMouseInput() override;

    glm::vec2 GetMousePosition(RenderSpace space = RenderSpace::Screen) override;
    glm::vec2 GetMouseDelta() override;

    float GetScrollDelta() override;

    void PollEvents() override;

    void SetMouseButton(int glfwButton, const Key &state) override;
    void SetKey(int glfwButton, const Key &state) override;

  private:
    void ResetMouseButtons();
    void ResetKeys();

    static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void CharacterCallback(GLFWwindow *window, unsigned int codepoint);

  private:
    std::map<int, Key> m_MouseButtons = {};
    std::map<int, Key> m_Keys = {};

    glm::vec2 m_CurrentMousePosition;
    glm::vec2 m_LastMousePosition;
    float m_Scroll;

    std::vector<std::function<void(float)>> m_ScrollCallbacks;

    std::string m_TextInputBuffer;
    std::vector<int> m_MouseInputBuffer;
};
