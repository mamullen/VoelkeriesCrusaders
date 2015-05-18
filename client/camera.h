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

	// Set viewport parameters
	void SetViewport(int x, int y, int width, int height);
	// Set projection prameters
	void SetProjection(float fovy, float aspect, float zNear, float zFar);

	// Add this pitch (rotation about the X-axis) in degrees
	// to the current camera's pitch 
	void AddPitch(float fPitch);
	// Add this yaw (rotation about the Y-axis) in degrees
	// to the current camera's yaw
	void AddYaw(float fYaw);
	// Add this roll (rotation about the Z-axis) in degrees 
	// to the current camera's roll
	void AddRoll(float fRoll);

	// Translate the along the camera's X-axis
	void TranslateX(float x);
	// Translate along the camera's Y-axis
	void TranslateY(float y);
	// Translate along the camera's Z-axis
	void TranslateZ(float z);

	// Set the current translation of the camera, replacing the current value.
	void SetTranslate(const Vector3& translate);
	// Set the current euler angles in degrees, replacing the current values.
	void SetRotate(float pitch, float yaw, float roll);
	void SetRotate(const Vector3& rotate);

	// Access functions
	void SetPosition(float x, float y, float z)		{ cx = x; cy = y; cz = z; }
	void SetAspect(float a)		{Aspect=a;}
	void SetDistance(float d)	{if(d>MIN_CAMERA_DISTANCE && d<MAX_CAMERA_DISTANCE) Distance=d;}
	void SetAzimuth(float a);
	void SetIncline(float i);

	float GetDistance()			{return Distance;}
	float GetAzimuth()			{return Azimuth;}
	float GetIncline()			{return Incline;}

	Vector3 GetTranslation() const;
	Vector3 GetRotation() const;

	void ApplyViewport();
	void ApplyProjectionTransform();

	void ApplyViewTransform();
private:
	Vector3 m_Rotate;
	Vector3 m_Translate;

	// Viewport parameters
	int         m_ViewportX;
	int         m_ViewportY;
	int         m_ViewportWidth;
	int         m_ViewportHeight;

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
