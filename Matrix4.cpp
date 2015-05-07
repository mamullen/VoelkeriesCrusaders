#include <math.h>

#include "Matrix4.h"
#include <string>

Matrix4::Matrix4()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
} 

Matrix4& Matrix4::operator=(const Matrix4& m2)
{
  if (this != &m2)
  {
    for (int i=0; i<4; ++i)
    {
      for (int j=0; j<4; ++j)
      {
        m[i][j] = m2.m[i][j];
      }
    }
  }
  return *this;
}


Matrix4 Matrix4::operator*(const Matrix4& m2)
{
	Matrix4 m3 = Matrix4();
			for (int i = 0; i<4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				for (int x = 0; x < 4; ++x)
				{

					m3.m[i][j] = m3.m[i][j] + (m[i][x] * m2.m[x][j]);
				}
			}
		}
	
	return m3;
}

Vector4 Matrix4::operator*(const Vector4& v)
{
	Vector4 v2 = Vector4();
	
		for (int i = 0; i<4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				
					switch (i)
					{
					case '1':
						v2.x = v2.x + (v.x * m[i][j]);
						break;
					case '2':
						v2.y = v2.y + (v.y * m[i][j]);
						break;
					case '3':
						v2.z = v2.z + (v.z * m[i][j]);
						break;
					case '4':
						v2.w = v2.w + (v.w * m[i][j]);
						break;
					
				}
			}
		}
	
     return v2;
}


// return pointer to matrix elements
double* Matrix4::getPointer()
{
  return &m[0][0];
}

// set matrix to identity matrix
void Matrix4::identity()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      if (i==j) 
        m[i][j] = 1.0; 
      else 
        m[i][j] = 0.0;
    }
  }
}

// transpose the matrix (mirror at diagonal)
void Matrix4::transpose()
{
  Matrix4 temp;
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      temp.m[j][i] = m[i][j];
    }
  }
  *this = temp;  // copy temporary values to this matrix
}



// Creates a rotation matrix which rotates about the y axis.
// angle is expected in degrees
void Matrix4::makeRotateY(double angle)
{
	
  angle = angle / 180.0 * M_PI;  // convert from degrees to radians
  identity();


	m[0][0] = cos(angle);
	m[0][2] = sin(angle);
	m[2][0] = -sin(angle);
	m[2][2] = cos(angle);
 

}

void Matrix4::makeRotateX(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[1][1] = cos(angle);
	m[1][2] = -sin(angle);
	m[2][1] = sin(angle);
	m[2][2] = cos(angle);
}

void Matrix4::makeRotateZ(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[0][0] = cos(angle);
	m[0][1] = -sin(angle);
	m[1][0] = sin(angle);
	m[1][1] = cos(angle);
}




void Matrix4::makeScale(double sx, double sy, double sz)
{

	identity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;

}

void Matrix4::makeTranslate(double tx, double ty, double tz)
{
	identity();
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
	
}



void Matrix4::print(std::string comment)
{
	printf("| %lf , %lf, %lf, %lf | \n", m[0][0], m[0][1], m[0][2], m[0][3]);
	printf("| %lf , %lf, %lf, %lf | \n", m[1][0], m[1][1], m[1][2], m[1][3]);
	printf("| %lf , %lf, %lf, %lf | \n", m[2][0], m[2][1], m[2][2], m[2][3]);
	printf("| %lf , %lf, %lf, %lf | \n %s\n", m[3][0], m[3][1], m[3][2], m[3][3], comment);
}

void Matrix4::makeInverse()
{
	//for (int i = 0; i <= 3; i++)
	//{
		for (int j = 0; j <= 3; j++)
		{
			m[j][0] = -m[j][0];
		}
	//}
}

void Matrix4::setXTrans(double x1, double x2, double x3)
{
	m[0][0] = x1;
	m[1][0] = x2;
	m[2][0] = x3;
	//m[3][0] = 0;

}

void Matrix4::setYTrans(double y1, double y2, double y3)
{
	m[0][1] = y1;
	m[1][1] = y2;
	m[2][1] = y3;
	//m[3][1] = 0;
}

void Matrix4::setZTrans(double z1, double z2, double z3)
{
	m[0][2] = z1;
	m[1][2] = z2;
	m[2][2] = z3;
	//m[3][2] = 0;
}

void Matrix4::setW()
{
	m[0][0] = 0;
	m[1][1] = 0;
	m[2][2] = 0;
	m[3][3] = 1;
}