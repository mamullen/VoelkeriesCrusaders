#include "stdafx.h"
#include "Vampire.h"


Vampire::Vampire() :Player()
{
	objectType = 5;
	team = 2;
	setAttack(new Basic_Attack());
}


Vampire::~Vampire()
{
}

Vampire::Vampire(int i) :Player(i)
{
	objectType = 5;
	team = 2;
}

void Vampire::updateTime(int time,int delta)
{
	
	if (time == 0){
		//day
		speed = default_speed;
		time_ctr += delta;
		if (time_ctr > 1000){
			time_ctr = 0;
			isAttacked(atof((ConfigSettings::config->getValue("VampireHpDropRate").c_str())));
		}
		if (attack_mode->getType() == 0){
			((Basic_Attack*)attack_mode)->resetDotAngle();
		}
	}
	else if (time == 1){
		//night
		speed = atof((ConfigSettings::config->getValue("VampireMovespeedMultiplier").c_str())) * default_speed;
		attack_mode->setRange(atof((ConfigSettings::config->getValue("VampireAttackRangeInc").c_str())));
		if (attack_mode->getType() == 0){
			((Basic_Attack*)attack_mode)->setDotAngle(1);
		}
	}
}

void Vampire::attack(GameObject* obj)
{
	if (this->id == obj->getID()){
		return;
	}
	if (this->inRange(obj)){
		obj->isAttacked(ad);
		if (obj->objectType == 3 && obj->getHP() >= 0){
			hp += 0.5*ad;
			if (hp > 100){
				hp = 100;
			}
		}
	}
}