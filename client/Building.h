#pragma once

#include <stdio.h>
#include "GameObject.h"

class Building : public GameObject{

public:
	Building(Vector3* min, Vector3* max, unsigned int i) : GameObject(NULL, min, max, i){ setMaxHealth(1); setHealth(1); };
	void drawObj();
};
