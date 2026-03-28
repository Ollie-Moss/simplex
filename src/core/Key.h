#pragma once

#include "GLFW/glfw3.h"
#include "core/KeyMod.h"

enum class KeyState : int {
    Released = GLFW_RELEASE,
    FirstPress = GLFW_PRESS,
    Repeat = GLFW_REPEAT,
    Held
};

struct Key
{
    Key(KeyState _state = KeyState::Released, KeyMod _mods = KeyMod::None) : state(_state), mods(_mods) {}

    bool IsPressed()
    {
        consumed = true;
        return state == KeyState::FirstPress;
    }

    bool IsPressedOrRepeating()
    {
        consumed = true;
        return state == KeyState::FirstPress || state == KeyState::Repeat;
    }

    bool IsDown()
    {
        return state == KeyState::FirstPress || state == KeyState::Held || state == KeyState::Repeat;
    }

    bool HasMods(KeyMod flag)
    {
        return (mods & flag) != KeyMod::None;
    }

    void Reset()
    {
        if(consumed && state == KeyState::FirstPress)
        {
            consumed = false;
            state = KeyState::Held;
        }

        if(consumed && state == KeyState::Repeat)
        {
            consumed = false;
            state = KeyState::Held;
        }
    }

  private:
    KeyState state = KeyState::Released;
    KeyMod mods = KeyMod::None;
    bool consumed = false;
};
