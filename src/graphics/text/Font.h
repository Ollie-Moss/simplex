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
    float maxAscent;
    float maxDescent;
    // from the baseline to the top of the line
    float lineHeight;
    // the gap from the baseline to the top of the next line
    float lineGap;
};

template <>
struct AssetLoader<Font>
{
    static Font Load(const Font::LoaderConfig &config);
};
