#pragma once
#include "GameObject.h"
#include "Action.h"
#include "Basic_Attack.h"
#include "ConfigSettings.h"
class Player : public GameObject
{
public:
	Player();
	Player(int i);// : GameObject(){ pid = i; };
	~Player();

	
	virtual void update(Packet*, std::vector<GameObject*>*);
	virtual void isAttacked(float);

	void updateCD();
	unsigned int getPID();
	
	bool inRange(GameObject*);
	void attack(GameObject*);
	void setAttack(Action*);

private:
	unsigned int pid; // corresponds to Client ID
	float ad;	// attack dmg
	Action* attack_mode; // determined by the weapon u picked
};

