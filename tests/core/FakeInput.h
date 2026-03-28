#pragma once
#include "core/IInput.h"
#include "glm/glm.hpp"
#include "graphics/util/RenderSpace.h"
#include <string>
#include <vector>
#include <unordered_map>

class FakeInput : public IInput
{
  public:
    FakeInput();
    ~FakeInput();

    // IInput
    bool Init(GLFWwindow *window) override;
    Key &GetKey(int glfwButton) override;
    Key &GetMouseButton(int glfwButton) override;
    std::string GetTextInput() override;
    const std::vector<int> &GetMouseInput() override;
    glm::vec2 GetMousePosition(RenderSpace space = RenderSpace::Screen) override;
    glm::vec2 GetMouseDelta() override;
    float GetScrollDelta() override;
    void PollEvents() override;
    void SetMouseButton(int glfwButton, const Key &state) override;
    void SetKey(int glfwButton, const Key &state) override;

    // Test helpers
    void SimulateKeyPress(int glfwKey);
    void SimulateMouseButton(int button);
    void SimulateScroll(float delta);
    void SimulateMousePosition(glm::vec2 pos);
    void SimulateTextInput(const std::string &text);

  private:
    std::unordered_map<int, Key> m_Keys;
    std::unordered_map<int, Key> m_MouseButtons;
    std::vector<int> m_MouseInputBuffer;
    std::string m_TextInputBuffer;
    float m_Scroll = 0;
    glm::vec2 m_CurrentMousePosition{};
    glm::vec2 m_LastMousePosition{};
};
