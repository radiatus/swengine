#pragma once

#include "OpenGL3.h"
#include <string>

typedef GLuint textureId;

class OpenGL3Texture {
public:
	OpenGL3Texture();
	~OpenGL3Texture();

	void loadFromFile(const std::string&);
	void unload();

	int getWidth() const;
	int getHeight() const;

	textureId getTexturePointer() const;
private:
	textureId m_textureId;

	int m_width;
	int m_height;
};

typedef OpenGL3Texture Texture;