#pragma once
#include "GameObject.h"
class Player : public GameObject
{
public:
	Player(int i) : GameObject(){ pid = i; };
	~Player();

	void update();
	unsigned int getPID();

private:
	unsigned int pid;
};

