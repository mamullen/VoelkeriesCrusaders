////////////////////////////////////////
// camera.cpp
////////////////////////////////////////

#include "camera.h"

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
	m_Rotate = Vector3(0);
	m_Translate = Vector3(0);
	Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Camera::SetViewport(int x, int y, int width, int height)
{
	m_ViewportX = x;
	m_ViewportY = y;
	m_ViewportWidth = width;
	m_ViewportHeight = std::max(height, 1);
}

////////////////////////////////////////////////////////////////////////////////

void Camera::SetProjection(float fovy, float aspect, float zNear, float zFar)
{
	FOV = fovy;
	Aspect = aspect;
	NearClip = zNear;
	FarClip = zFar;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::AddPitch(float fPitch)
{
	m_Rotate.x += fPitch;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::AddYaw(float fYaw)
{
	m_Rotate.y += fYaw;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::AddRoll(float fRoll)
{
	m_Rotate.z += fRoll;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::TranslateX(float x)
{
	m_Translate.x += x;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::TranslateY(float y)
{
	m_Translate.y += y;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::TranslateZ(float z)
{
	m_Translate.z += z;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::SetTranslate(const Vector3& translate)
{
	m_Translate = translate;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::SetRotate(float pitch, float yaw, float roll)
{
	m_Rotate = Vector3(pitch, yaw, roll);
}

////////////////////////////////////////////////////////////////////////////////

void Camera::SetRotate(const Vector3& rotate)
{
	m_Rotate = rotate;
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

void Camera::SetAzimuth(float a){
	while (a < 0){
		a += 360;
	}
	while (a >= 360){
		a -= 360;
	}
	m_Rotate.y = a;
	Azimuth = a;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::SetIncline(float i){
	if (i > MIN_CAMERA_INCLINE && i < MAX_CAMERA_INCLINE) {
		m_Rotate.x = i;
		Incline = i;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Reset() {
	Distance=0.0f;
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

Vector3 Camera::GetRotation() const {
	return m_Rotate;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Camera::GetTranslation() const
{
	return m_Translate;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::ApplyViewport()
{
	glViewport(m_ViewportX, m_ViewportY, m_ViewportWidth, m_ViewportHeight);
}

////////////////////////////////////////////////////////////////////////////////

void Camera::ApplyProjectionTransform()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, Aspect, NearClip, FarClip);
}

////////////////////////////////////////////////////////////////////////////////

osg::Matrixd Camera::GetApplyViewTransform()
{
	osg::Matrixd cameraMx;
	osg::Matrixd cameraRotation;
	osg::Matrixd cameraTrans;
	cameraRotation.makeRotate(
		osg::DegreesToRadians(m_Rotate.x), osg::Vec3(1, 0, 0), // pitch
		osg::DegreesToRadians(m_Rotate.y), osg::Vec3(0, 1, 0), // roll
		osg::DegreesToRadians(m_Rotate.z), osg::Vec3(0, 0, 1)); // heading

	cameraTrans.makeTranslate(m_Translate.x, m_Translate.y, m_Translate.z);
	cameraMx = cameraRotation * cameraTrans;

	return cameraMx.inverse(cameraMx);
}

void Camera::ApplyViewTransform() 
{
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(m_Translate.x, m_Translate.y, m_Translate.z);
	glRotatef(m_Rotate.x, 1.0f, 0.0f, 0.0f);
	glRotatef(m_Rotate.y, 0.0f, 1.0f, 0.0f);
	glRotatef(m_Rotate.z, 0.0f, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////
