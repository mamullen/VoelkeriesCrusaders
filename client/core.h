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

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WINDOWTITLE	"Voelkeries Crusaders"

#define M_PI	3.14159f

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
