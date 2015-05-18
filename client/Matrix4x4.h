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
	double m[4][4];   // matrix elements; first index is for rows, second for columns (row-major)

public:
	Matrix4x4();
	Matrix4x4(double angle, int which);
	Matrix4x4(double x, double y, double z, int which);
	Matrix4x4(double &x0, double &x1, double &x2, double &x3,
		double &y0, double &y1, double &y2, double &y3,
		double &z0, double &z1, double &z2, double &z3,
		double w0, double w1, double w2, double w3);
	Matrix4x4& operator=(const Matrix4x4&);
	Matrix4x4& operator*(const Matrix4x4&);
	Vector4& operator*(const Vector4&);
	double* getPointerByRowMajor();
	double* getPointerByColMajor();
	void identity();
	void transpose();
	void makeRotateX(double);
	void makeRotateY(double);
	void makeRotateZ(double);
	void makeRotate(double, const Vector3&);
	void makeScale(double, double, double);
	void makeTranslate(double, double, double);
};

#endif
