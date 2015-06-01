#ifndef VAMPPLAYER_H_
#define VAMPPLAYER_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "MeshLoader.h"
#include "objparser.h"
#include "GameObject.h"
#include "PlayerType.h"
#include <iostream>
#include "ConfigSettings.h"

class VampirePlayer : public PlayerType {
public:
	VampirePlayer(unsigned int i, int t) : PlayerType(new MeshLoader((char*)ConfigSettings::config->getValue("VampFilePath").c_str()), i, t) { 
		setShowHP(true);
		p_Class = Vampire;
		team = t; 
	}
	int getTeam()		{ return team; }

private:
	int team;
};

#endif