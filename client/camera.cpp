////////////////////////////////////////
// camera.cpp
////////////////////////////////////////

#include "camera.h"

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
	Reset();
    cx=0;
    cy=-5;
    cz=0;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Update(bool LeftDown, bool RightDown, int dx, int dy) {
	const float rate = 1.0f;

	if (LeftDown) {
		SetAzimuth(GetAzimuth() + dx*rate);
		SetIncline(GetIncline() - dy*rate);
	}
	if (RightDown) {
		SetAzimuth(GetAzimuth() + dx*rate);
		SetIncline(GetIncline() - dy*rate);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Reset() {
	FOV=60.0f;
	Aspect=1.33f;
	NearClip=0.1f;
	FarClip=100.0f;

	Distance=20.0f;
	Azimuth=0.0f;
	Incline=0.0f;
} 

////////////////////////////////////////////////////////////////////////////////

void Camera::Draw() {
	// Tell GL we are going to adjust the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set perspective projection
	gluPerspective(FOV,Aspect,NearClip,FarClip);

	// Place camera
	glTranslatef(cx, cy, cz - Distance);
	glRotatef(Incline,1.0f,0.0f,0.0f);
	glRotatef(Azimuth,0.0f,1.0f,0.0f);
}

////////////////////////////////////////////////////////////////////////////////
