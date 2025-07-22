#pragma once

#include "graphics/Character.h"
#include <map>

struct Font {
    std::map<char, Character> characters;
};
