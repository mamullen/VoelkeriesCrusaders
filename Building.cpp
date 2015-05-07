//
//  Building.cpp
//  cse169
//
//  Created by michael mullen jr on 4/10/15.
//
//

#include "Building.h"
Building::Building()
{
    maxy=0;
    x1=0;
    x2=0;
    x3=0;
    x4=0;//corners of the box for x
    z1=0;
    z2=0;
    z3=0;
    z4=0;//corners of the box for z

}

Building::Building(float y, float ex1, float ex2, float ze1, float ze2)
{
    maxy=y;
    x1=ex1;
    x2=ex2;
    z1=ze1;
    z2 = ze2;
}