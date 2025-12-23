#include "AssetManager.h"
#include "graphics/util/Shader.h"

bool AssetManager::Init()
{
    Load<Shader>("SpriteShader", {.vertexShaderPath = "vSpriteShader.glsl", .fragmentShaderPath = "fSpriteShader.glsl"});
    Load<Shader>("TextShader", {.vertexShaderPath = "vTextShader.glsl", .fragmentShaderPath = "fTextShader.glsl"});
    Load<Shader>("DefaultShader", {.vertexShaderPath = "vDefaultShader.glsl", .fragmentShaderPath = "fDefaultShader.glsl"});

    return true;
}
