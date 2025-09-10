#include "ResourceManager.h"
#include "graphics/Font.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#define SHADER_PATH

const Shader &ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, std::string name)
{
    Shader shader;
    shader.Compile(vShaderFile, fShaderFile);
    m_Shaders[name] = shader;
    return m_Shaders[name];
}
bool ResourceManager::Init()
{
    stbi_set_flip_vertically_on_load(true);

    LoadShader(std::string(std::string(SHADER_PATH) + "vSpriteShader.glsl").c_str(), std::string(std::string(SHADER_PATH) + "fSpriteShader.glsl").c_str(), "SpriteShader");
    LoadShader(std::string(std::string(SHADER_PATH) + "vTextShader.glsl").c_str(), std::string(std::string(SHADER_PATH) + "fTextShader.glsl").c_str(), "TextShader");

    //LoadTexture("GRASS_TILE_1", 1.0f, "sprites/grass_tile_1.png");
    // Buildings

    // Fonts
    //LoadFont("Arial", "fonts/arial.ttf");
    return true;
};

const Shader &ResourceManager::GetShader(std::string name)
{
    return m_Shaders[name];
}

const Texture &ResourceManager::LoadTexture(std::string name, bool alpha, const char *file)
{
    // create texture object
    Texture texture = Texture();
    if(alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    // load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, STBI_rgb_alpha);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    m_Textures[name] = texture;
    return m_Textures[name];
}

const Texture &ResourceManager::GetTexture(std::string name)
{
    return m_Textures[name];
}

const Font &ResourceManager::LoadFont(std::string name, std::string path)
{
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    if(FT_New_Face(ft, path.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    FT_Set_Pixel_Sizes(face, 0, 50);
    if(FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for(unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                               (unsigned int)face->glyph->advance.x};
        m_Fonts[name].characters.insert(std::pair<char, Character>(c, character));
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return m_Fonts[name];
}

const Font &ResourceManager::GetFont(std::string name)
{
    return m_Fonts[name];
};

void ResourceManager::Clear()
{
    // (properly) delete all shaders
    for(auto iter : m_Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for(auto iter : m_Textures)
        glDeleteTextures(1, &iter.second.ID);
}
