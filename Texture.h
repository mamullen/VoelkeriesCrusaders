#ifndef _TEXTURE_H_
#define _TEXTURE_H

#include <stdlib.h>
#include <stdio.h>
//#include "GLee.h"
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
using namespace std;
class Texture
{
public:
	Texture();
	~Texture();
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	//void initGL();
	void loadTexture(const char* name, GLuint id);
	//void LoadTextureCubeMap(char **FileNames, GLuint id);
	//GLuint load(vector<const GLchar*> faces);
	

};

#endif

