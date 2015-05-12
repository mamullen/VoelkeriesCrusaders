//
//  Building.h
//  cse169
//
//  Created by michael mullen jr on 4/10/15.
//
//

#ifndef __cse169__Building__
#define __cse169__Building__

#include <stdio.h>

class Building{
public:
    Building();
    float maxy;
    float x1,x2,x3,x4;//corners of the box for x
    float z1,z2,z3,z4;//corners of the box for z
    Building(float y, float ex1, float ex2, float ze1, float z2);
    
};

#endif /* defined(__cse169__Building__) */
