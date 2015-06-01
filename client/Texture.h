#ifndef TEXT_H_
#define TEXT_H_

#include "core.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

/* Try uncommenting this include line to compile on Max OSX
#include <GLUT/glut.h>
*/

#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

GLuint loadDDS(const char * imagepath);

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& filename);
	bool Load();
	void Bind(GLenum TextureUnit);
	Texture();
	Texture(GLenum tt, const char * fn);
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	//void initGL();
	void loadTexture(const char* name, GLuint id);
	GLuint loadDDS(const char * imagepath);
	void initTexture(GLenum tt, const char * fn);
private:
	std::string m_filename;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	const char* filename;
};

#endif