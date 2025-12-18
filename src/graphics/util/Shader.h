#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assets/AssetLoader.h>

class Shader
{
  public:
    struct LoaderConfig
    {
        std::string vertexShaderPath;
        std::string fragmentShaderPath;
    };

    unsigned int ID;

    Shader() = default;
    Shader(const char *vertexPath, const char *fragmentPath);
    void Compile(const char *vertexPath, const char *fragmentPath);
    void use() const;

    // Utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;
};

template <>
struct AssetLoader<Shader>
{
    static Shader Load(const Shader::LoaderConfig &config);
};
