//
//  Vector4.h
//  cse169
//
//  Created by michael mullen jr on 4/11/15.
//
//

#ifndef __cse169__Vector4__
#define __cse169__Vector4__

#include <stdio.h>
#include <string>
#include <math.h>

#include "vector3.h"

class Vector4
{
public:
	void Set(float x0, float y0, float z0, float w0)				{ x = x0; y = y0; z = z0; w = w0; }
	Vector4();
	Vector4(float f) : x(f), y(f), z(f), w(f) {}
    ~Vector4();
    Vector4(double x, double y, double z, double w);
	Vector4 operator*(float f) const;
    Vector4 operator+(const Vector4& v);
    Vector4 operator-(const Vector4& v);
    void dehomogenize();
    void print(std::string comment);
	double length();
    void normalize();
    double dot(const Vector4& v1, const Vector4& v2);
    Vector4 cross(const Vector4& v1, const Vector4& v2);
    
    Vector4 scale(double s);

public:
	float x, y, z, w;
    
};



#endif /* defined(__cse169__Vector4__) */
