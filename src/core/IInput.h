#pragma once

#include "core/Key.h"
#include "glm/glm.hpp"
#include "graphics/util/RenderSpace.h"
#include <string>
#include <vector>

class IInput
{
  public:
    // IInput() = default;
    // virtual ~IInput() = default;

    virtual bool Init(GLFWwindow *window) = 0;

    virtual Key &GetKey(int glfwButton) = 0;
    virtual Key &GetMouseButton(int glfwButton) = 0;

    // Returns a string of all text input received in the last frame
    virtual std::string GetTextInput() = 0;

    // Returns a list of all mouse buttons pressed in the last frame in chronological order
    virtual const std::vector<int> &GetMouseInput() = 0;

    virtual glm::vec2 GetMousePosition(RenderSpace space = RenderSpace::Screen) = 0;
    virtual glm::vec2 GetMouseDelta() = 0;

    virtual float GetScrollDelta() = 0;

    virtual void PollEvents() = 0;

    // Sets the state of a given mouse button
    virtual void SetMouseButton(int glfwButton, const Key &state) = 0;

    // Sets the state of a given key
    virtual void SetKey(int glfwButton, const Key &state) = 0;

};
