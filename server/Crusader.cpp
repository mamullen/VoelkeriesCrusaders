#include "stdafx.h"
#include "Crusader.h"


Crusader::Crusader() :Player()
{
	objectType = 4;
	team = 1;
	default_attack_1 = new Basic_Range();
	default_attack_2 = new Basic_Knock();
	setAttack(default_attack_1);
}


Crusader::~Crusader()
{
}

Crusader::Crusader(int i) :Player(i)
{
	objectType = 4;
	team = 1;
	default_attack_1 = new Basic_Range();
	default_attack_2 = new Basic_Knock();
	setAttack(default_attack_1);
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