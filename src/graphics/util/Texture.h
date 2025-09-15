#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Texture {
  public:
	// holds the ID of the texture object, used for all texture operations to
	// reference to this particular texture
	unsigned int ID;
	// texture image dimensions
	unsigned int Width, Height; // width and height of loaded image in pixels
	// texture Format
	unsigned int Internal_Format; // format of texture object
	unsigned int Image_Format;	  // format of loaded image
	// texture configuration
	unsigned int Wrap_S;	 // wrapping mode on S axis
	unsigned int Wrap_T;	 // wrapping mode on T axis
	unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
	unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
							 
	std::vector<unsigned char> textureData;

	// constructor (sets default texture modes)
	Texture();
	// generates texture from image data
	void Generate(unsigned int width, unsigned int height, unsigned char *data);

	// Gets the rgba values of a given texture coordiante
	glm::vec4 GetPixel(glm::vec2 texCoord) const;

	// binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};
