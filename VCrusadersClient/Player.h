#pragma once
#include "GameObject.h"
class Player : public GameObject
{
public:
	Player(int i) : GameObject(i){};
	~Player();

	void update();
};

