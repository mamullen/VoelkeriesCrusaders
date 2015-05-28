#include "stdafx.h"
#include "Vampire.h"


Vampire::Vampire()
{
}


Vampire::~Vampire()
{
}

Vampire::Vampire(int i) :Player(i)
{

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