#include "Font.h"
#include <format>
#include <iostream>
#include <ostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ft2build.h"
#include FT_FREETYPE_H

Font AssetLoader<Font>::Load(const Font::LoaderConfig &config)
{
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    if(FT_New_Face(ft, config.path.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    FT_Set_Pixel_Sizes(face, 0, 16);
    if(FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    Font font;
    FT_Size_Metrics &m = face->size->metrics;

    float ascent = m.ascender / 64.0f;
    float descent = -m.descender / 64.0f;
    float height = m.height / 64.0f;
    float lineGap = height - (ascent + descent);

    font.maxAscent = ascent;
    font.maxDescent = descent;
    font.lineHeight = ascent + descent + lineGap;

    lineGap = std::max(0.0f, lineGap);
    font.lineHeight = ascent + descent + lineGap;
    std::cout << std::format("height: {}, gap: {}, ascent: {}, descent: {}", font.lineHeight, lineGap, ascent, descent) << "\n";

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
                               (unsigned int)(face->glyph->advance.x / 64.0f)};
        font.characters.insert(std::pair<char, Character>(c, character));
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return font;
};
