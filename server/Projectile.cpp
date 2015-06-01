#include "stdafx.h"
#include "Projectile.h"
#include "GameLogic.h"

Projectile::Projectile()
{
	speed =0.1;
	range = 200;
	modifier = 1;
	hitRadius = 5;
}


Projectile::~Projectile()
{
}

bool Projectile::updateTime(int time)
{
	position = position + speed*time/1000*(forward.Normalize());
	distance += speed*time;
	//printf("distance traveld = %f\n", distance);

	for (int i = 0; i < GameLogic::playerList.size(); i++){
		if (collide(GameLogic::playerList[i])){
			printf("target hit!\n");
			GameLogic::playerList[i]->isAttacked(dmg * modifier);
			return false;
		}
	}
	if (distance >= range){
		printf("died D=\n");
		return false;
	}
	return true;
}

bool Projectile::collide(GameObject* obj)
{
	if ((obj->getPos() - position).Mag() <= hitRadius){
		float dot = forward.Dot(obj->getPos() - position);
		if (dot < 0){
			return false;
		}
		if (dot > 0.8){
			modifier = 1.5;
		}
		else if (dot > 0.6){
			modifier = 1;
		}
		else{
			modifier = dot;
		}
		return true;
	}
	return false;
}