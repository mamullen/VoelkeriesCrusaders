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

	void setHDir(int h) { printf("%d\n", h); hDir = h; }
	void setVDir(int v) { printf("%d\n", v); vDir = v; }
	int getHDir()		{ return hDir; }
	int getVDir()		{ return vDir; }

protected:
	PlayerClass p_Class;

private:
	int team;
	//vDir: 1 = forward, -1 = backward, 0 = neither
	//hDir: 1 = right, -1 = left, 0 = neither
	int vDir=0, hDir=0;

};

#endif