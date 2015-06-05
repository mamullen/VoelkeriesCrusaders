#include "stdafx.h"
#include "Vampire.h"


Vampire::Vampire() :Player()
{
	objectType = 5;
	team = 2;
	default_attack_1 = new Basic_Attack();
	default_attack_2 = new Power_Range();
	//default_attack_1 = new Basic_Range();
	//default_attack_2 = new Power_Range();
	setAttack(new Basic_Attack());
}


Vampire::~Vampire()
{
}

Vampire::Vampire(int i) :Player(i)
{
	objectType = 5;
	team = 2;
	default_attack_1 = new Basic_Attack();
	default_attack_2 = new Power_Range();
	//default_attack_1 = new Basic_Range();
	//default_attack_2 = new Power_Range();
	//default_attack_3 = new Power_Range();
	setAttack(new Basic_Attack());
}

void Vampire::updateTime(int time, int delta, std::vector<GameObject*>* obj)
{
	if (dashCounter > 0){
		speed = dashSpeed;
		forward = dashDir;
		if (!collide(obj, forward))
			moveForward();
		//speed = prevSpeed;
		std::string* change = new std::string("pos:");
		changes.push_back(std::pair<int, std::string*>(id, change));
		dashCounter--;
		return;
	}

	if (dashCharging){
		speedDebuff += atof((ConfigSettings::config->getValue("VampireChargeSpeedDebuff").c_str()));
		dashRange += atof((ConfigSettings::config->getValue("VampireChargeDistance").c_str()));
		double maxDashRange = atof((ConfigSettings::config->getValue("VampireChargeMaxDistance").c_str()));
		if (dashRange > maxDashRange){
			dashRange = maxDashRange;
		}
		std::string* change = new std::string("dashRange");
		changes.push_back(std::pair<int, std::string*>(id, change));
	}

	if (time == 0){
		//day
		speed = atof((ConfigSettings::config->getValue("VampireMovespeedBaseMult").c_str())) * default_speed;
		speed -= speedDebuff;
		if (speed<0){
			speed = atof((ConfigSettings::config->getValue("VampireLowestMovespeed").c_str()));
		}
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
		speed -= speedDebuff;
		if (speed < 0){
			speed = atof((ConfigSettings::config->getValue("VampireLowestMovespeed").c_str()));
		}
		attack_mode->setRange(atof((ConfigSettings::config->getValue("VampireAttackRangeInc").c_str())));
		if (attack_mode->getType() == 0){
			((Basic_Attack*)attack_mode)->setDotAngle(0);
		}
	}
}

void Vampire::attack2Start(){
	if (dashCounter > 0)
		return;
	dashCharging = true;
}

void Vampire::attack2End(){

	if (dashCounter > 0 || !dashCharging)
		return;
	//set to new pos at end of dash

	prevSpeed = speed;
	/*
	moveForward();
	speed = prevSpeed;
	*/


	dashSpeed = dashRange / dashDivider;
	dashDir = forward;
	dashCounter = dashDivider;

	speedDebuff = 0;
	dashRange = 0;
	dashCharging = false;
	std::string* change = new std::string("dashRange");
	changes.push_back(std::pair<int, std::string*>(id, change));

}

//void Vampire::attack(GameObject* obj) // does nothing right now
//{
//	if (this->id == obj->getID()){
//		return;
//	}
//	if (this->inRange(obj)){
//		obj->isAttacked(ad);
//		if (obj->objectType == 3 && obj->getHP() >= 0){
//			hp += 0.5*ad;
//			if (hp > 100){
//				hp = 100;
//			}
//		}
//	}
//}