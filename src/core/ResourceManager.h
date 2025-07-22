#pragma once

#include <map>
#include "graphics/Shader.h"
#include "graphics/Font.h"
#include "graphics/Texture.h"
#include <string>

class ResourceManager {
  public:
    // Instantiate static variables
    std::map<std::string, Texture> Textures;
    std::map<std::string, Shader> Shaders;
    std::map<std::string, Font> Fonts;

    bool Init();

    const Shader &LoadShader(const char *vShaderFile, const char *fShaderFile, std::string name);

    const Shader &GetShader(std::string name);

    const Texture &LoadTexture(std::string name, bool alpha, const char *file);

    const Texture &GetTexture(std::string name);

    const Font &LoadFont(std::string name, std::string path);

    const Font &GetFont(std::string name);

    void Clear();
};
