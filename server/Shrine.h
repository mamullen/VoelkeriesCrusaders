#pragma once
#include "GameObject.h"
#include "Action.h"
#include "Basic_Attack.h"
#include "ConfigSettings.h"
class Shrine : public GameObject
{
public:
	Shrine();
	Shrine(int i);// : GameObject(){ pid = i; };
	~Shrine();


	virtual void update(Packet*, std::vector<GameObject*>*);


	unsigned int getPID();

	

private:
	unsigned int pid; // corresponds to Client ID
	float ad;	// attack dmg
	Action* attack_mode; // determined by the weapon u picked
};