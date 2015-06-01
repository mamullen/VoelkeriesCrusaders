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

enum PlayerClass { Human, Vampire, Crusader };

class PlayerType: public GameObject {
public:
	PlayerType(MeshLoader* model, unsigned int i, int t) : GameObject(model, i) { setShowHP(true); team = t; p_Class = Human; };
	//Player(unsigned int i, int t) : GameObject(NULL, i){ setShowHP(true); team = t; };
	PlayerClass getClass() { return p_Class; }

	int getTeam()		{ return team; }

protected:
	PlayerClass p_Class;

private:
	int team;
};

#endif