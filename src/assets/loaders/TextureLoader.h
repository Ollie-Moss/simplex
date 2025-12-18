#pragma once

#include "graphics/util/Texture.h"
#include "assets/AssetLoader.h"

#include <string>
#include "stb_image.h"

struct TextureConfig
{
    std::string path;
    bool alpha = true;
};

template <>
struct AssetLoader<TextureConfig>
{
    static Texture Load(const TextureConfig &config)
    {
        Texture texture = Texture();
        if(config.alpha)
        {
            texture.Internal_Format = GL_RGBA;
            texture.Image_Format = GL_RGBA;
        }

        int width, height, nrChannels;
        unsigned char *data = stbi_load(config.path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

        texture.Generate(width, height, data);

        stbi_image_free(data);
        return texture;
    };
};
