#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			m[i][j] = 0;
		}
	}
	identity();
}

Matrix4x4::Matrix4x4(float angle, int which)
{
	if (which == 0) {
		makeRotateY(angle);
	}
	else if (which == 1) {
		makeRotateX(angle);
	}
}

Matrix4x4::Matrix4x4(float x, float y, float z, int which)
{
	if (which == 0) {
		identity();
	}
	else if (which == 1) { // translate
		makeTranslate(x, y, z);
	}
	else if (which == 2) { // scale
		makeScale(x, y, z);
	}
}

Matrix4x4::Matrix4x4(float &x0, float &x1, float &x2, float &x3,
	float &y0, float &y1, float &y2, float &y3,
	float &z0, float &z1, float &z2, float &z3,
	float w0, float w1, float w2, float w3) {
	this->m[0][0] = x0; this->m[0][1] = x1; this->m[0][2] = x2; this->m[0][3] = x3;
	this->m[1][0] = y0; this->m[1][1] = y1; this->m[1][2] = y2; this->m[1][3] = y3;
	this->m[2][0] = z0; this->m[2][1] = z1; this->m[2][2] = z2; this->m[2][3] = z3;
	this->m[3][0] = w0; this->m[3][1] = w1; this->m[3][2] = w2; this->m[3][3] = w3;
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& m2)
{
	if (this != &m2)
	{
		for (int i = 0; i<4; ++i)
		{
			for (int j = 0; j<4; ++j)
			{
				m[i][j] = m2.m[i][j];
			}
		}
	}
	translation = Vector3(m2.m[0][3], m2.m[1][3], m2.m[2][3]);
	scale = Vector3(m2.m[0][0], m2.m[1][1], m2.m[2][2]);
	return *this;
}

Matrix4x4& Matrix4x4::operator*(const Matrix4x4& m2)
{
	float sum = 0;
	Matrix4x4 tempM;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			for (int j = 0; j < 4; j++)
			{
				sum = sum + m[row][j] * m2.m[j][col];
				tempM.m[row][col] = sum;
			}
			sum = 0;
		}
	}

	return *this = tempM;
	//row major and column major
}

Vector4& Matrix4x4::operator*(const Vector4& v)
{
	Vector4 result;
	float sum = 0;
	float vect[4];
	float tempV[4];
	vect[0] = v.x;
	vect[1] = v.y;
	vect[2] = v.z;
	vect[3] = v.w;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sum += m[i][j] * vect[j];
		}
		tempV[i] = sum;
		sum = 0;
	}
	//printf("called *: %f, %f, %f, %f \n", tempV[0], tempV[1], tempV[2], tempV[3]);
	result = Vector4(tempV[0], tempV[1], tempV[2], tempV[3]);
	return result;
}

// return pointer to matrix elements
float* Matrix4x4::getPointerByRowMajor()
{
	return &m[0][0];
}

float* Matrix4x4::getPointerByColMajor()
{
	transpose();
	return &m[0][0];
}

// set matrix to identity matrix
void Matrix4x4::identity()
{
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			if (i == j)
				m[i][j] = 1.0;
			else
				m[i][j] = 0.0;
		}
	}
}

// transpose the matrix (mirror at diagonal)
void Matrix4x4::transpose()
{
	Matrix4x4 temp;
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			temp.m[j][i] = m[i][j];
		}
	}
	*this = temp;  // copy temporary values to this matrix
}


// Creates a rotation matrix which rotates about the x axis.
// angle is expected in degrees
void Matrix4x4::makeRotateX(float angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[1][1] = cos(angle);
	m[1][2] = -sin(angle);
	m[2][1] = sin(angle);
	m[2][2] = cos(angle);
}


// Creates a rotation matrix which rotates about the y axis.
// angle is expected in degrees
void Matrix4x4::makeRotateY(float angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[0][0] = cos(angle);
	m[0][2] = sin(angle);
	m[2][0] = -sin(angle);
	m[2][2] = cos(angle);
}

// Creates a rotation matrix which rotates about the z axis.
// angle is expected in degrees
void Matrix4x4::makeRotateZ(float angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[0][0] = cos(angle);
	m[0][1] = -sin(angle);
	m[1][0] = sin(angle);
	m[1][1] = cos(angle);
}

void Matrix4x4::makeRotate(float angle, const Vector3& axis)
{
	angle = angle / 180.0 * M_PI; // convert from degrees to radians
	identity(); //we need to normalize to prevent shearing (put axis in temp, then normalize the temp)
	Vector3 temp = axis;
	temp.Normalize();
	// change here after declaring friend class
	m[0][0] = pow(temp.x, 2) + cos(angle) * (1 - pow(temp.x, 2));
	m[0][1] = temp.x * temp.y * (1 - cos(angle)) - temp.z * sin(angle);
	m[0][2] = temp.x * temp.z * (1 - cos(angle)) + temp.y * sin(angle);
	m[1][0] = temp.x * temp.y * (1 - cos(angle)) + temp.z * sin(angle);
	m[1][1] = pow(temp.y, 2) + cos(angle) * (1 - pow(temp.y, 2));
	m[1][2] = temp.y * temp.z * (1 - cos(angle)) - temp.x * sin(angle);
	m[2][0] = temp.x * temp.z * (1 - cos(angle)) - temp.y * sin(angle);
	m[2][1] = temp.y * temp.z * (1 - cos(angle)) + temp.x * sin(angle);
	m[2][2] = pow(temp.z, 2) + cos(angle) * (1 - pow(temp.z, 2));
}

void Matrix4x4::makeScale(float sx, float sy, float sz)
{
	identity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
}

void Matrix4x4::makeTranslate(float tx, float ty, float tz)
{
	identity();
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
}