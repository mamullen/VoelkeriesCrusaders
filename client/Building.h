#pragma once

#include <stdio.h>
#include "GameObject.h"

class Building : public GameObject{

public:
	Building(unsigned int i) : GameObject(NULL, i){};
	void drawObj();
};
