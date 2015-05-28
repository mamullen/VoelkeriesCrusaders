#include "stdafx.h"
#include "Vampire.h"


Vampire::Vampire() :Player()
{
	objectType = 4;
}


Vampire::~Vampire()
{
}

Vampire::Vampire(int i) :Player(i)
{
	objectType = 4;
}

void Vampire::updateTime(int time,int delta)
{
	
	if (time == 0){
		//day
		speed = default_speed;
		time_ctr += delta;
		if (time_ctr > 1000){
			time_ctr = 0;
			isAttacked(1);
		}
	}
	else if (time == 1){
		//night
		speed = 2 * default_speed;
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