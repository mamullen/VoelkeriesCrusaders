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

	virtual void updateTime(int,int);
private:
	float HpDropRate = (float)atoi(ConfigSettings::config->getValue("VampireHpDropRate").c_str());
};

