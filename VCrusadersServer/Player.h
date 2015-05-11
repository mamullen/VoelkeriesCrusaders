#pragma once
#include "GameObject.h"
#include "BasicAttack.h"
class Player : public GameObject
{
public:
	Player();
	Player(int i);// : GameObject(){ pid = i; };
	~Player();

	void update(Packet*);
	unsigned int getPID();
	
	void basic_attack(GameObject&);

	Action* basic_att;
private:
	unsigned int pid; // corresponds to Client ID
	
};

