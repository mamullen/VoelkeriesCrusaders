#ifndef PLAYER_H_
#define PLAYER_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "DaeLoader.h"
#include "objparser.h"
#include "GameObject.h"
#include <iostream>
#include "ConfigSettings.h"

class Player: public GameObject {
public:
	Player(unsigned int i, int t) : GameObject(new DaeLoader((char*)ConfigSettings::config->getValue("MechAnimationFilePath").c_str()), i){ setShowHP(true); team = t; };
	//Player(unsigned int i) : GameObject(NULL, i){ setShowHP(true); };

	int getTeam()		{ return team; }

private:
	int team;

};

#endif