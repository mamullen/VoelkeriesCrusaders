#ifndef TEXT_H_
#define TEXT_H_

#include "core.h"
#include <iostream>

GLuint loadDDS(const char * imagepath);

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
};

#endif