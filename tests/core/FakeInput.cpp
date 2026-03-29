#include "FakeInput.h"

FakeInput::FakeInput() {}
FakeInput::~FakeInput() {}

bool FakeInput::Init(GLFWwindow *window) { return true; }

// Keyboard
Key &FakeInput::GetKey(int glfwButton) { return m_Keys[glfwButton]; }
void FakeInput::SetKey(int glfwButton, const Key &key) { m_Keys[glfwButton] = key; }

// Mouse buttons
Key &FakeInput::GetMouseButton(int glfwButton) { return m_MouseButtons[glfwButton]; }
void FakeInput::SetMouseButton(int glfwButton, const Key &key) { m_MouseButtons[glfwButton] = key; }

// Text / scroll
std::string FakeInput::GetTextInput() { return m_TextInputBuffer; }
const std::vector<int> &FakeInput::GetMouseInput() { return m_MouseInputBuffer; }
float FakeInput::GetScrollDelta() { return m_Scroll; }

// Mouse position
glm::vec2 FakeInput::GetMousePosition(RenderSpace space) { return m_CurrentMousePosition; }
glm::vec2 FakeInput::GetMouseDelta() { return m_LastMousePosition - m_CurrentMousePosition; }

void FakeInput::PollEvents()
{
    for(auto &[btn, key] : m_Keys)
        key.Reset();
    for(auto &[btn, key] : m_MouseButtons)
        key.Reset();
    m_Scroll = 0;
    m_LastMousePosition = m_CurrentMousePosition;
    m_TextInputBuffer.clear();
    m_MouseInputBuffer.clear();
}

// Test helpers
void FakeInput::SimulateKeyPress(int glfwKey)
{
    m_Keys[glfwKey] = Key(KeyState::FirstPress, KeyMod::None);
}

void FakeInput::SimulateMouseButton(int button)
{
    m_MouseButtons[button] = Key(KeyState::FirstPress, KeyMod::None);
    m_MouseInputBuffer.push_back(button);
}

void FakeInput::SimulateScroll(float delta) { m_Scroll = delta; }

void FakeInput::SimulateMousePosition(glm::vec2 pos) { m_CurrentMousePosition = pos; }

void FakeInput::SimulateTextInput(const std::string &text) { m_TextInputBuffer += text; }
