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
#include <algorithm>
#include <boost/algorithm/clamp.hpp>

#ifdef _WIN32
	#include <SOIL/SOIL.h>
	#include <GL/glew.h>
	#include <GLFW/glfw3.h>
#endif

#define WINDOWTITLE	"Voelkeries Crusaders"

#define M_PI	3.14159f
#define toRadians(d) (d * M_PI/180)
#define toDegrees(r) (r * 180/M_PI)
#define lerp(x, y, a) (x*(1.0-a)+y*a)
#define saturate(x) (boost::algorithm::clamp(x, 0, 1))

void drawAxis(float size);
void drawWireBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);

static const int MIN_CAMERA_DISTANCE = 7;
static const int MAX_CAMERA_DISTANCE = 20;
static const int MIN_CAMERA_INCLINE = 0;
static const int MAX_CAMERA_INCLINE = 85;

static const int ROTATE_LEFT = 10;
static const int ROTATE_RIGHT = -10;

////////////////////////////////////////////////////////////////////////////////

/*
This file just has a bunch of common stuff used by all objects. It mainly just
includes GL and some other standard headers.
*/

#endif
