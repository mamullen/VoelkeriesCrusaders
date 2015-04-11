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

#define M_PI 3.14159265358979323846
class Vector4
{
public:
    double x;
    double y;
    double z;
    double w;
    Vector4();
    ~Vector4();
    Vector4(double x, double y, double z, double w);
    Vector4 operator+(const Vector4& v);
    Vector4 operator-(const Vector4& v);
    void dehomogenize();
    void print(std::string comment);
    float length();
    void normalize();
    double dot(const Vector4& v1, const Vector4& v2);
    Vector4 cross(const Vector4& v1, const Vector4& v2);
    
    void scale(double s);
    
    
};



#endif /* defined(__cse169__Vector4__) */
