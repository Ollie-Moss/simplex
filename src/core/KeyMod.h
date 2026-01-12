#pragma once

#include "GLFW/glfw3.h"

enum class KeyMod : int {
    None = 0,
    Shift = GLFW_MOD_SHIFT,
    Control = GLFW_MOD_CONTROL,
    Alt = GLFW_MOD_ALT,
    Super = GLFW_MOD_SUPER,
    CapsLock = GLFW_MOD_CAPS_LOCK,
    NumLock = GLFW_MOD_NUM_LOCK
};

inline KeyMod operator|(KeyMod a, KeyMod b)
{
    return static_cast<KeyMod>(
        static_cast<int>(a) | static_cast<int>(b));
}

inline KeyMod operator&(KeyMod a, KeyMod b)
{
    return static_cast<KeyMod>(
        static_cast<int>(a) & static_cast<int>(b));
}

inline KeyMod &operator|=(KeyMod &a, KeyMod b)
{
    return a = a | b;
}
