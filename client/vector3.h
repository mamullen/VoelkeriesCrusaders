////////////////////////////////////////
// vector3.h
////////////////////////////////////////

#ifndef CSE169_VECTOR3_H
#define CSE169_VECTOR3_H

#include "core.h"
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

class Vector3;

class Quaternion {
public:
	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(const float &w, const float &x, const float &y, const float &z) : x(x), y(y), z(z), w(w) {}
	Quaternion(const Vector3& eulerAngle);
	Quaternion(float angle, Vector3& axis);
	Vector3 operator*(const Vector3& v) const;
	Quaternion operator/ (float s);
	Quaternion Inverse(const Quaternion& q);
	Quaternion Conjugate(const Quaternion& q);
	float Length(const Quaternion& q);
	float Dot(const Quaternion& q1, const Quaternion& q2);
public:
	float x, y, z, w;
};

class Vector3 {
public:
	Vector3 () : x (0.0f), y (0.0f), z (0.0f)			{}
	Vector3 (float f) : x (f), y (f), z (f)				{}
	Vector3 (float x0, float y0, float z0) :
		x (x0), y (y0), z (z0)							{}
	void Set (float x0, float y0, float z0)				{x = x0; y = y0; z = z0;}
	void Zero ()										{x = y = z = 0.0f;}

	// Algebra
	//-----------------------------------------------------------------------
	//@{ \name Addition.
	//-----------------------------------------------------------------------
	//! Component-wise vector addition operator.
	Vector3 operator+ (const Vector3& v) const
	{
		return Vector3 (x + v.x, y + v.y, z + v.z);
	}
	
	//! Component-wise vector addition-assignment operator.
	const Vector3 & operator+= (const Vector3& v)
	{
		x += v.x; y += v.y; z += v.z; return *this;
	}
	//@}
	
	
	//-----------------------------------------------------------------------
	//@{ \name Subtraction.
	//-----------------------------------------------------------------------
	//! Component-wise vector subtraction operator.
	Vector3 operator- (const Vector3& v) const
	{
		return Vector3 (x - v.x, y - v.y, z - v.z);
	}
	
	//! Component-wise vector subtraction-assignment operator.
	const Vector3 & operator-= (const Vector3& v)
	{
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	//@}
	
	
	//-----------------------------------------------------------------------
	//@{ \name Multiplication.
	//-----------------------------------------------------------------------
	//! Scalar multiplication operator.
	Vector3 operator* (float a) const {return Vector3 (x * a, y * a, z * a);}
	
	//! Component-wise vector multiplication operator.
	Vector3 operator* (const Vector3& v) const
	{
		return Vector3 (x * v.x, y * v.y, z * v.z);
	}

	Vector3 operator* (const Quaternion& q) const
	{
		Quaternion q_inv;
		q_inv = q_inv.Inverse(q);
		return q_inv * *this;
	}
	
	//! Scalar multiplication-assignment operator.
	const Vector3 & operator*= (float a) {x *= a; y *= a; z *= a; return *this;}
	
	//! Component-wise vector multiplication-assignment operator.
	const Vector3 & operator*= (const Vector3& v)
	{
		x *= v.x; y *= v.y; z *= v.z; return *this;
	}
	
	//! Negation operator.
	Vector3 operator- () const {return Vector3 (-x, -y, -z);}
	const Vector3 & Negate () {x = -x; y = -y; z = -z; return *this;}
	//@}
	
	//-----------------------------------------------------------------------
	//@{ \name Division.
	//-----------------------------------------------------------------------
	//! Scalar division operator.
	Vector3 operator/ (float a) const
	{
		float inv = 1.0f / a;
		return Vector3 (x * inv, y * inv, z * inv);
	}
	
	//! Component-wise vector division operator.
	Vector3 operator/ (const Vector3 & v) const
	{
		return Vector3 (x / v.x, y / v.y, z / v.z);
	}
	
	//! Scalar division-assignment operator.
	const Vector3 & operator/= (float a)
	{
		float inv = 1.0f / a;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}
	
	//! Component-wise vector division-assignment operator.
	const Vector3 & operator/= (const Vector3 & v)
	{
		x /= v.x; y /= v.y; z /= v.z; return *this;
	}
	//@}
	
	
	
	//-----------------------------------------------------------------------
	//@{ \name Length, normalization, etc.
	//-----------------------------------------------------------------------
	float Mag2 () const {return x * x + y * y + z * z;}
	float Mag () const {return sqrtf (Mag2 ());}
	const Vector3 & Normalize () {return (*this /= Mag ());}
	//@}

	float Dot(const Vector3 & a) const					{return x*a.x+y*a.y+z*a.z;}
	void Cross(const Vector3 & a,const Vector3 & b)		{x=a.y*b.z-a.z*b.y; y=a.z*b.x-a.x*b.z; z=a.x*b.y-a.y*b.x;}
	void Lerp(float t,const Vector3 a,const Vector3 b)	{*this = a; *this *= 1.0f-t; x+=b.x*t; y+=b.y*t; z+=b.z*t;}

	float Dist2(const Vector3 a) const					{return (x-a.x)*(x-a.x)+(y-a.y)*(y-a.y)+(z-a.z)*(z-a.z);}

	// Misc functions
	void Print(char *name=0) const						{if(name) printf("%s=",name); printf("{%f,%f,%f}\n",x,y,z);}

	float &operator[](int i)							{return(((float*)this)[i]);}

	// Static vectors
	static Vector3 XAXIS,YAXIS,ZAXIS;
	static Vector3 ORIGIN;
	static Vector3 g_DefaultCameraTranslate, g_DefaultCameraRotate;

	Vector3 inRadians() {
		return Vector3(toRadians(x), toRadians(y), toRadians(z));
	}

	Vector3 vCos() {
		return Vector3(cos(x), cos(y), cos(z));
	}
public:
	float x, y, z;
};


//! Multiply a scalar by a Vector3.
inline Vector3 operator* (float s, const Vector3& v)
{
    return Vector3 (v.x * s, v.y * s, v.z * s);
}

////////////////////////////////////////////////////////////////////////////////

#endif
