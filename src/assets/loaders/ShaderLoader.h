#pragma once
#include "graphics/util/Shader.h"
#include "assets/AssetLoader.h"

struct ShaderConfig
{
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};

template <>
struct AssetLoader<Shader>
{
    static Shader Load(const ShaderConfig &config)
    {
        Shader shader;
        shader.Compile(config.vertexShaderPath.c_str(), config.fragmentShaderPath.c_str());
        return shader;
    };
};
