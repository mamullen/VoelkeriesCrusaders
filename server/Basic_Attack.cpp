#include "stdafx.h"
#include "Basic_Attack.h"
#include "GameObject.h"

Basic_Attack::Basic_Attack()
{
	cd = 0;
	maxcd = (float)atoi(ConfigSettings::config->getValue("PlayerAttackCD").c_str());
	ad = (float)atoi(ConfigSettings::config->getValue("PlayerAttackDMG").c_str());
	range = (float)atoi(ConfigSettings::config->getValue("PlayerAttackRange").c_str());
	c_ad = ad;
	c_range = range;
}


Basic_Attack::~Basic_Attack()
{
}

float Basic_Attack::getDmg()
{
	return c_ad;
}

float Basic_Attack::getRange()
{
	return c_range;
}

void Basic_Attack::setRange(float m){
	c_range = range * m;
}

void Basic_Attack::attack(GameObject* obj,GameObject* target)
{
	if (obj->getID() == target->getID()){
		return;
	}
	if (inRange(obj,target)){
		target->isAttacked(getDmg());
	}
}

bool Basic_Attack::inRange(GameObject* obj,GameObject* target)
{
	float dot = (target->getPos() - obj->getPos()).Normalize().Dot(obj->forward.Normalize());
	float distance = (target->getPos() - obj->getPos()).Mag();
	if (dot < 0.8){
		return false;
	}
	if (distance > getRange()){
		return false;
	}
	return true;
}

