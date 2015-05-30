#include "stdafx.h"
#include "Projectile.h"
#include "GameLogic.h"

Projectile::Projectile()
{
	speed =1;
	range = 200;
}


Projectile::~Projectile()
{
}

bool Projectile::updateTime(int time)
{
	position = position + speed*time/1000*forward;
	distance += speed*time;
	if (distance >= range){
		return false;
	}
	return true;
}