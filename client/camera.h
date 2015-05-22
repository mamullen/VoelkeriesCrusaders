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
};

////////////////////////////////////////////////////////////////////////////////

/*
The Camera class provides a simple means to controlling the 3D camera. It could
be extended to support more interactive controls. Ultimately. the camera sets the
GL projection and viewing matrices.
*/

#endif
