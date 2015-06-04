#pragma once
#include "Player.h"
#include "ConfigSettings.h"

class Vampire :
	public Player
{
public:
	Vampire();
	Vampire(int i);
	virtual ~Vampire();
	void attack2Start();
	void attack2End();
	virtual void updateTime(int, int, std::vector<GameObject*>*);

	int dashDivider = atof(ConfigSettings::config->getValue("VampireDashDivider").c_str());
	int dashCounter = 0;
	float dashSpeed = 0;
	double prevSpeed = 0;
	Vector3 dashDir;

	double getDashRange()	{ return dashRange; }
	//virtual void attack(GameObject*);
private:
	float HpDropRate = atof(ConfigSettings::config->getValue("VampireHpDropRate").c_str());
	double speedDebuff = 0;
	double dashRange = 0;
	bool dashCharging = false;
};

