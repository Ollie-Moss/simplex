#pragma once

#include "Character.h"
#include <map>

struct Font {
    std::map<char, Character> characters;
};
