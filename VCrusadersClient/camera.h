////////////////////////////////////////
// camera.h
////////////////////////////////////////

#ifndef CSE169_CAMERA_H
#define CSE169_CAMERA_H

#include "core.h"
#include "Vector4.h"

////////////////////////////////////////////////////////////////////////////////

class Camera {
public:
	Camera();
    float cx,cy,cz;
	void Update(bool LeftDown, bool RightDown, int dx, int dy);
	void Reset();
	void Draw();

	// Access functions
	void SetPosition(float x, float y, float z)		{ cx = x; cy = y; cz = z; }
	void SetAspect(float a)		{Aspect=a;}
	void SetDistance(float d)	{if(d>MIN_CAMERA_DISTANCE && d<MAX_CAMERA_DISTANCE) Distance=d;}
	void SetAzimuth(float a);
	void SetIncline(float i)	{if(i>MIN_CAMERA_INCLINE && i<MAX_CAMERA_INCLINE) Incline=i;}

	float GetDistance()			{return Distance;}
	float GetAzimuth()			{return Azimuth;}
	float GetIncline()			{return Incline;}

private:
	// Perspective controls
	float FOV;		// Field of View Angle
	float Aspect;	// Aspect Ratio
	float NearClip;	// Near clipping plane distance
	float FarClip;	// Far clipping plane distance

	// Polar controls
	float Distance;	// Distance of the camera eye position to the origin
	float Azimuth;	// Rotation of the camera eye position around the Y axis
	float Incline;	// Angle of the camera eye position over the XZ plane
};

////////////////////////////////////////////////////////////////////////////////

/*
The Camera class provides a simple means to controlling the 3D camera. It could
be extended to support more interactive controls. Ultimately. the camera sets the
GL projection and viewing matrices.
*/

#endif
