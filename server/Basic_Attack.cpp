#include "stdafx.h"
#include "Basic_Attack.h"
#include "GameObject.h"
#include "Player.h"

Basic_Attack::Basic_Attack()
{
	type = 0;
	cd = 0;
	maxcd = stof(ConfigSettings::config->getValue("PlayerAttackCD").c_str());
	ad = stof(ConfigSettings::config->getValue("PlayerAttackDMG").c_str());
	range = stof(ConfigSettings::config->getValue("PlayerAttackRange").c_str());
	c_ad = ad;
	c_range = range;
	default_dotAngle = stof(ConfigSettings::config->getValue("BasicAngle").c_str());
	dotAngle = default_dotAngle;
}


Basic_Attack::~Basic_Attack()
{
}

void Basic_Attack::attack(GameObject* obj,GameObject* target)
{
	//printf("obj type = %d\n",obj->objectType);
	if (obj->getID() == target->getID()){
		return;
	}
	if (inRange(obj,target)){
		if (target->objectType == 4 && target->getHP() >= 0){
			obj->addHp(0.8*c_ad);
		}
		//printf("basic attack in range!!!\n");
		if (target->isPlayer && obj->isPlayer){
			Player * p1 = (Player*)obj;
			Player * p2 = (Player*)target;
			if (p1->team != p2->team){
				target->isAttacked(getDmg());
			}
		}
	}
}

void Basic_Attack::attack()
{

}
bool Basic_Attack::inRange(GameObject* obj,GameObject* target)
{
	float dot = (target->getPos() - obj->getPos()).Normalize().Dot(obj->forward.Normalize());
	float distance = (target->getPos() - obj->getPos()).Mag();
	if (dot < dotAngle){
		return false;
	}
	if (distance > getRange()){
		return false;
	}
	return true;
}

