#pragma once
#include "GameObject.h"
class Player : public GameObject
{
public:
	Player();
	Player(int i);// : GameObject(){ pid = i; };
	~Player();

	void update(Packet*);
	unsigned int getPID();

private:
	unsigned int pid; // corresponds to Client ID
	float hp;
	float attack_range;
	float attack_dmg;
};

