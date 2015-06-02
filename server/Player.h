#pragma once
#include "GameObject.h"
#include "Action.h"
#include "Basic_Attack.h"
#include "Basic_Range.h"
#include "ConfigSettings.h"
class Player : public GameObject
{
public:
	Player();
	Player(int i);// : GameObject(){ pid = i; };
	~Player();

	
	virtual void update(Packet*, std::vector<GameObject*>*);
	virtual void isAttacked(float);
	virtual void updateTime(int,int); // first int is day/night, second int is time elapsed

	void updateCD();
	void resetDir()			{ hDir = 0; vDir = 0; }
	void checkChangedDir();
	unsigned int getPID();
	int getHDir()			{ return hDir; }
	int getVDir()			{ return vDir; }
	
	bool inRange(GameObject*);
	//virtual void attack(GameObject*);
	void setAttack(Action*);
	bool collide(std::vector<GameObject*>* obj, Vector3 dir);
	bool shrinecollide(Vector3, Vector3);
	bool weaponcollide(Vector3);
	string name; 
	int team; // 0: no team, 1: crusader, 2: vampire

protected:
	unsigned int pid; // corresponds to Client ID
	float ad;	// attack dmg
	Action* attack_mode; // determined by the weapon u picked
	bool isAlive;
	int time_ctr;
	int hDir, vDir, hDirPrev, vDirPrev; //Let other players know which way this player is trying to move
};

