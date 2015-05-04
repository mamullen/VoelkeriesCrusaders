////////////////////////////////////////
// core.h
////////////////////////////////////////

#ifndef _CORE_H
#define _CORE_H

////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
	#define M_PI	3.14159f

	#include <assimp/ai_assert.h>
	#include <assimp/Importer.hpp>
	#include <assimp/cimport.h>
	#include <assimp/scene.h>
	#include <assimp/postprocess.h>
	#define GLFW_INCLUDE_GLU
	#include <GLFW/glfw3.h>
#endif

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#endif

#define WINDOWTITLE	"Voelkeries Crusaders"
#define ANIMATION_TICKS_PER_SECOND 20.0

void drawAxis(float size);
void drawWireBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);

#define MIN_CAMERA_DISTANCE 7
#define MAX_CAMERA_DISTANCE 20
#define MIN_CAMERA_INCLINE 0
#define MAX_CAMERA_INCLINE 85

#define ROTATE_LEFT 10
#define ROTATE_RIGHT -10

////////////////////////////////////////////////////////////////////////////////

/*
This file just has a bunch of common stuff used by all objects. It mainly just
includes GL and some other standard headers.
*/

#endif
