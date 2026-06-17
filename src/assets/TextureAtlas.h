#pragma once

#include "glm/fwd.hpp"
#include "graphics/util/Texture.h"
class TextureAtlas
{
  public:
    struct LoaderConfig
    {
        std::string path;
        bool alpha = true;
    };
    struct TextureLocation
    {
        glm::vec2 position;
        glm::vec2 size;
    };

  public:
    std::unordered_map<std::string, TextureLocation> locations;
    Texture texture;

  private:
};

template <>
struct AssetLoader<TextureAtlas>
{
    static TextureAtlas Load(const TextureAtlas::LoaderConfig &config);
};
