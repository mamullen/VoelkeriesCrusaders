#ifndef MATRIX4_H_
#define MATRIX4_H_

#include "core.h"
#include <string>
#include "vector4.h"
#include "vector3.h"

class Matrix4x4
{
protected:
	Vector3 translation;
	Vector3 scale;
	float m[4][4];   // matrix elements; first index is for rows, second for columns (row-major)

public:
	Matrix4x4();
	Matrix4x4(float angle, int which);
	Matrix4x4(float x, float y, float z, int which);
	Matrix4x4(float &x0, float &x1, float &x2, float &x3,
		float &y0, float &y1, float &y2, float &y3,
		float &z0, float &z1, float &z2, float &z3,
		float w0, float w1, float w2, float w3);
	Matrix4x4& operator=(const Matrix4x4&);
	Matrix4x4& operator*(const Matrix4x4&);
	Vector4& operator*(const Vector4&);
	float* getPointerByRowMajor();
	float* getPointerByColMajor();
	void identity();
	void transpose();
	void makeRotateX(float);
	void makeRotateY(float);
	void makeRotateZ(float);
	void makeRotate(float, const Vector3&);
	void makeScale(float, float, float);
	void makeTranslate(float, float, float);
};

#endif
