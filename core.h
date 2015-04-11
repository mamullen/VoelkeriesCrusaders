////////////////////////////////////////
// core.h
////////////////////////////////////////

#ifndef _CORE_H
#define _CORE_H

////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	#define M_PI	3.14159f
	#include <math.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <ctype.h>

	#include <windows.h>
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif

#ifdef __APPLE__
	#include <math.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <ctype.h>

	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#endif

void drawAxis(float size);
void drawWireBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);

////////////////////////////////////////////////////////////////////////////////

/*
This file just has a bunch of common stuff used by all objects. It mainly just
includes GL and some other standard headers.
*/

#endif
