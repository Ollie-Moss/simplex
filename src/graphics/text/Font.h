#pragma once

#include "Character.h"
#include <map>
#include <string>
#include <assets/AssetLoader.h>
#include <string>


struct Font
{
    struct LoaderConfig
    {
        std::string path;
    };

    std::map<char, Character> characters;
};

template <>
struct AssetLoader<Font>
{
    static Font Load(const Font::LoaderConfig &config);
};
