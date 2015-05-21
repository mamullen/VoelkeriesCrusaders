//
//  Vector4.cpp
//  cse169
//
//  Created by michael mullen jr on 4/11/15.
//
//

#include "Vector4.h"


Vector4::Vector4()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
    w = 0.0;
}


Vector4::~Vector4()
{
}

Vector4::Vector4(double x, double y, double z, double w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w; //pass one array and pass one pointer ver
}

Vector4 Vector4::operator*(float f) const
{
	return Vector4(x * f, y * f, z * f, w);
}

Vector4 Vector4::operator+(const Vector4& v)
{
	return Vector4(x + v.x, y + v.y, z + v.z, w);
}

Vector4 Vector4::operator-(const Vector4& v)
{
	return Vector4(x - v.x, y - v.y, z - v.z, w);
}

double Vector4::length()
{
    return sqrt(x*x + y*y + z*z);
}

void Vector4::normalize()
{
    double l = this->length();
    x = x/l;
    y = y/l;
    z = z/l;
}

void Vector4::dehomogenize()
{
    //w -> 1 turns on translation else w -> 0 off translation
    x = x / w;
    y = y / w;
    z = z / w;
    w = w / w;
}

double Vector4::dot(const Vector4& v1, const Vector4& v2)
{
    return ((v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z));
}

Vector4 Vector4::cross(const Vector4& v1, const Vector4& v2)
{
    
    this->x = (v1.y*v2.z) - (v1.z*v2.y);
    this->y = (v1.z*v2.x) - (v1.x*v2.z);
    this->z = (v1.x*v2.y) - (v1.y*v2.x);
    
    return *this;
}

Vector4 Vector4::scale(double s)
{
	return Vector4(s*x, s*y, s*z, w);
}