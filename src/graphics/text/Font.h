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

    std::map<char32_t, Character> characters;
    float maxAscent;
    float maxDescent;
    float lineHeight;
    float lineGap;
};

template <>
struct AssetLoader<Font>
{
    static Font Load(const Font::LoaderConfig &config);
};
