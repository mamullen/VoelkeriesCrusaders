////////////////////////////////////////
// matrix34.h
////////////////////////////////////////

#ifndef CSE169_MATRIX34_H
#define CSE169_MATRIX34_H

#include "vector3.h"

////////////////////////////////////////////////////////////////////////////////

class Matrix34 {
public:
	Matrix34()										{Identity(); pad0=pad1=pad2=0.0f; pad3=1.0f;}
	Matrix34(float ax,float bx,float cx,float dx,
		 float ay,float by,float cy,float dy,
		 float az,float bz,float cz,float dz);

	void Identity();

	// Dot
	void Dot(const Matrix34 &n,const Matrix34 &m);	// this = n (dot) m

	// Transform
	void Transform(const Vector3 &in,Vector3 &out) const;
	void Transform3x3(const Vector3 &in,Vector3 &out) const;

	// MakeRotate (NOTE: t is an angle in RADIANS)
	void MakeRotateX(float t);
	void MakeRotateY(float t);
	void MakeRotateZ(float t);
	void MakeRotateUnitAxis(const Vector3 &v,float t);	// v must be normalized

	// Scale
	void MakeScale(float x,float y,float z);
	void MakeScale(const Vector3 &v)				{MakeScale(v.x,v.y,v.z);}
	void MakeScale(float s)							{MakeScale(s,s,s);}

	// Translate
	void MakeTranslate(float x,float y,float z)		{Identity(); d.Set(x,y,z);}
	void MakeTranslate(const Vector3 &v)			{Identity(); d=v;}

	// Euler angles
	enum {EULER_XYZ,EULER_XZY,EULER_YXZ,EULER_YZX,EULER_ZXY,EULER_ZYX};
	void FromEulers(float ang1, float ang2, float ang3, int order);
	void ToEulers(float* ang1, float* ang2, float* ang3, int order);

	// Inversion
	bool Inverse();									// Full inverse (expensive)
	void FastInverse();								// Only works on ORTHONORMAL matrices
	void Transpose();								// Only modifies 3x3 portion (doesn't change d)

	// Viewing
	void LookAt(const Vector3 &from,const Vector3 &to);
	void PolarView(float dist,float azm,float inc,float twst=0);

	// Misc functions
	float Determinant3x3() const;
	void Print(const char *s=0) const;
	Vector3 &operator[](int i)						{return *((Vector3*)&(((float*)this)[i*4]));}	// yuck!
	operator float*()								{return (float*)this;}

	// Static matrices
	static Matrix34 IDENTITY;

public:
	Vector3 a; float pad0;
	Vector3 b; float pad1;
	Vector3 c; float pad2;
	Vector3 d; float pad3;
};

////////////////////////////////////////////////////////////////////////////////

/*
The Matrix34 is a more optimized version of a 4x4 matrix. In memory, it sits
like a 4x4 matrix due to the 4 pad variables which are set to 0,0,0,1.

There are 4 vectors which can be useful for geometric operations. Think of the
'a' vector as pointing to the object's right, 'b' pointing to the object's top,
and 'c' pointing to the objects back. Usually 'a', 'b', and 'c' will be unit
length and perpendicular to each other. 'd' represents the object's position.

There are various functions for creating standard matrices (rotation, translation,
scale...) as well as some other algebra functions (inverse, transpose...).

Because the Matrix34 sits in memory the same way as a 4x4 matrix, it can be
passed directly to OpenGL with the glMultMatrixf(mtx) or glLoadMatrixf(mtx)
commands. The Matrix34 specifies an implicit float* cast operation to make
this work.
*/

#endif
