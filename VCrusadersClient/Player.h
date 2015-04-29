#ifndef PLAYER_H_
#define PLAYER_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "objparser.h"
#include "GameObject.h"
#include <iostream>

class Player: public GameObject {
public:
	Player() : GameObject(new ObjParser("Object/MechAnimation.obj")){};
};

#endif