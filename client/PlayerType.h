#ifndef PLAYERTYPE_H_
#define PLAYERTYPE_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "MeshLoader.h"
#include "objparser.h"
#include "GameObject.h"
#include <iostream>
#include "ConfigSettings.h"

class PlayerType: public GameObject {
public:
	PlayerType(MeshLoader* model, unsigned int i, int t) : GameObject(model, i){ setShowHP(true); team = t; };
	//Player(unsigned int i, int t) : GameObject(NULL, i){ setShowHP(true); team = t; };

	int getTeam()		{ return team; }

private:
	int team;
};

#endif