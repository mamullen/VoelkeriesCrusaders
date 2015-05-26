////////////////////////////////////////
// vector3.cpp
////////////////////////////////////////

#include "vector3.h"

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::XAXIS(1.0f,0.0f,0.0f);
Vector3 Vector3::YAXIS(0.0f,1.0f,0.0f);
Vector3 Vector3::ZAXIS(0.0f,0.0f,1.0f);
Vector3 Vector3::ORIGIN(0.0f, 0.0f, 0.0f);
Vector3 Vector3::g_DefaultCameraTranslate(0, 0, -20);
Vector3 Vector3::g_DefaultCameraRotate(40, 0, 0);

////////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion(const Vector3& eulerAngle) {
	Vector3 c = (eulerAngle * 0.5).vCos();
	Vector3 s = (eulerAngle * 0.5).vSin();

	this->w = c.x * c.y * c.z + s.x * s.y * s.z;
	this->x = s.x * c.y * c.z - c.x * s.y * s.z;
	this->y = c.x * s.y * c.z + s.x * c.y * s.z;
	this->z = c.x * c.y * s.z - s.x * s.y * c.z;
}

Quaternion::Quaternion(float angle, const Vector3& axis) {
	const Vector3 axis1 = axis.Normalize();

	float a = toRadians(angle);
	float s = sin(a*0.5);

	this->w = cos(a*0.5);
	this->x = axis1.x * s;
	this->y = axis1.y * s;
	this->z = axis1.z * s;
}

Vector3 Quaternion::operator*(const Vector3& v) const {
	Vector3 uv, uuv;
	Vector3 q_vec(x, y, z);
	uv.Cross(q_vec, v);
	uuv.Cross(q_vec, uv);
	uv *= (2 * w);
	uuv *= 2;

	return v + uv + uuv;
}

Quaternion Quaternion::operator/ (float s) {
	return Quaternion(w / s, x / s, y / s, z / s);
}

Quaternion Quaternion::Inverse(const Quaternion& q) {
	return Conjugate(q) / Length(q);
}

Quaternion Quaternion::Conjugate(const Quaternion& q) {
	return Quaternion(q.w, -q.x, -q.y, -q.z);
}

float Quaternion::Length(const Quaternion& q) {
	return sqrt(Dot(q, q));
}

float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2) {
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}
