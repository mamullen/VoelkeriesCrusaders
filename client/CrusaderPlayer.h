#ifndef CRUPLAYER_H_
#define CRUPLAYER_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "MeshLoader.h"
#include "objparser.h"
#include "GameObject.h"
#include "PlayerType.h"
#include <iostream>
#include "ConfigSettings.h"

class CrusaderPlayer : public PlayerType {
public:
	CrusaderPlayer(unsigned int i, int t) : PlayerType(new MeshLoader((char*)ConfigSettings::config->getValue("CrusaderFilePath").c_str()), i, t){ setShowHP(true); team = t; }
	int getTeam()		{ return team; }

private:
	int team;
};

#endif