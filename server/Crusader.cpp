#include "stdafx.h"
#include "Crusader.h"


Crusader::Crusader() :Player()
{
	objectType = 4;
}


Crusader::~Crusader()
{
}

Crusader::Crusader(int i) :Player(i)
{
	objectType = 4;
	team = 1;
}

void Crusader::updateTime(int time, int delta)
{
	if (time == 0){
		//day
		attack_mode->resetRange();
	}
	else if (time == 1){
		//night
		attack_mode->setRange(0.8);
	}
}

void Crusader::attack(GameObject* obj)
{
	if (this->id == obj->getID()){
		return;
	}
	if (this->inRange(obj)){
		if (obj->objectType != 3){
			obj->isAttacked(ad);
		}
	}
}