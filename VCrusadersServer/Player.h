#pragma once
#include "GameObject.h"
#include "BasicAttack.h"

class Player : public GameObject
{
public:
	Player();
	Player(int i);// : GameObject(){ pid = i; };
	~Player();

	virtual void update(Packet*, std::vector<GameObject*>*);
	unsigned int getPID();

	void setAttackMode(Action*);
	void attack(std::vector<GameObject*>*);
	bool inRange(Player*);
	void isAttacked(float);
	float getHp();
private:
	float hp;
	float attack_range;
	float attack_dmg;
	unsigned int pid; // corresponds to Client ID
	Action* attack_mode;
};

