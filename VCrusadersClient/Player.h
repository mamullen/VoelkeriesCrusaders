#ifndef PLAYER_H_
#define PLAYER_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "objparser.h"
#include "GameObject.h"
#include <iostream>
#include "ConfigSettings.h"

class Player: public GameObject {
public:
	Player(unsigned int i) : GameObject(new ObjParser( (char*) ConfigSettings::config->getValue("MechAnimationFilePath").c_str() ),i){};
};

#endif