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
	
	void basic_attack(GameObject*);

private:
	unsigned int pid; // corresponds to Client ID
	
};

