//
//  Floor.h
//  cse169
//
//  Created by michael mullen jr on 4/5/15.
//
//

#ifndef __cse169__Floor__
#define __cse169__Floor__

#include "core.h"
#include "vector3.h"

#include "Building.h"
#include <iostream>
#include <vector>

class Floor{
public:
    Floor(float);
    Floor();
    void createTile(Building* b, float offsetx, float offsetz);
    std::vector<Building*> buildingList;
    void createFloor(float offsetx, float offsetz);
    float exits;
    float tilenumber;
    
    float floorsizex=20;
    float floorsizey=-5;
    float floorsizez=20;
    GLuint texture[5];
    bool state;
   // Text t;
    
};


#endif /* defined(__cse169__Floor__) */
