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
	PlayerType(unsigned int id, int t) : GameObject(id) { setShowHP(true); team = t; };
	//Player(unsigned int i, int t) : GameObject(NULL, i){ setShowHP(true); team = t; };

	int getTeam()		{ return team; }

	void setHDir(int h) { printf("%d\n", h); hDir = h; }
	void setVDir(int v) { printf("%d\n", v); vDir = v; }
	int getHDir()		{ return hDir; }
	int getVDir()		{ return vDir; }

private:
	int team;
	//vDir: 1 = forward, -1 = backward, 0 = neither
	//hDir: 1 = right, -1 = left, 0 = neither
	int vDir=0, hDir=0;

};

#endif