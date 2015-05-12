#ifndef TEXT_H_
#define TEXT_H_

#include "core.h"
#include <iostream>

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& filename);
	bool Load();
	void Bind(GLenum TextureUnit);

private:
	std::string m_filename;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	/*Magick::Image m_image;
	Magick::Blob m_blob;*/
};

#endif