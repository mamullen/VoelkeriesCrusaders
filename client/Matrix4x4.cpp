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

Matrix4x4::Matrix4x4(double angle, int which)
{
	if (which == 0) {
		makeRotateY(angle);
	}
	else if (which == 1) {
		makeRotateX(angle);
	}
}

Matrix4x4::Matrix4x4(double x, double y, double z, int which)
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

Matrix4x4::Matrix4x4(double &x0, double &x1, double &x2, double &x3,
	double &y0, double &y1, double &y2, double &y3,
	double &z0, double &z1, double &z2, double &z3,
	double w0, double w1, double w2, double w3) {
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
	double sum = 0;
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
	double sum = 0;
	double vect[4];
	double tempV[4];
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
double* Matrix4x4::getPointerByRowMajor()
{
	return &m[0][0];
}

double* Matrix4x4::getPointerByColMajor()
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
void Matrix4x4::makeRotateX(double angle)
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
void Matrix4x4::makeRotateY(double angle)
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
void Matrix4x4::makeRotateZ(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[0][0] = cos(angle);
	m[0][1] = -sin(angle);
	m[1][0] = sin(angle);
	m[1][1] = cos(angle);
}

void Matrix4x4::makeRotate(double angle, const Vector3& axis)
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

void Matrix4x4::makeScale(double sx, double sy, double sz)
{
	identity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
}

void Matrix4x4::makeTranslate(double tx, double ty, double tz)
{
	identity();
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
}