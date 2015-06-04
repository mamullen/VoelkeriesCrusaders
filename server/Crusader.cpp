#include "stdafx.h"
#include "Crusader.h"


Crusader::Crusader() :Player()
{
	objectType = 4;
	team = 1;
	setAttack(new Basic_Range());
}


Crusader::~Crusader()
{
}

Crusader::Crusader(int i) :Player(i)
{
	objectType = 4;
	team = 1;
	setAttack(new Basic_Range());
}

void Crusader::updateTime(int time, int delta, std::vector<GameObject*>* obj)
{
	if (time == 0){
		//day
		attack_mode->setRange(atof((ConfigSettings::config->getValue("CrusaderAttackRangeInc").c_str())));
		speed = atof((ConfigSettings::config->getValue("CrusaderMovespeedDay").c_str())) * default_speed;
	}
	else if (time == 1){
		//night
		attack_mode->setRange(atof((ConfigSettings::config->getValue("CrusaderAttackRangeDec").c_str())));
		speed = atof((ConfigSettings::config->getValue("CrusaderMovespeedNight").c_str())) * default_speed;
	}
}

//void Crusader::attack(GameObject* obj) // does nothing right now
//{
//	if (this->id == obj->getID()){
//		return;
//	}
//	if (this->inRange(obj)){
//		if (obj->objectType != 3){
//			obj->isAttacked(ad);
//		}
//	}
//}