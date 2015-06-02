#include "stdafx.h"
#include "Projectile.h"
#include "GameLogic.h"

Projectile::Projectile()
{
	speed =50;
	range = 200;
	modifier = 1;
	hitRadius = 6;
}


Projectile::~Projectile()
{
}

bool Projectile::updateTime(int time)
{
	position = position + speed*time/1000*(forward.Normalize());
	distance += speed*time/1000;
	
	int hit = 0;
	for (int i = 0; i < GameLogic::playerList.size(); i++){
		if (collide(GameLogic::playerList[i])){
			printf("target hit!\n");
			GameLogic::playerList[i]->isAttacked(dmg * modifier);
			hit++;
		}
	}
	if (hit > 0){
		return false;
	}
	if (distance >= range){
		printf("died D=\n");
		return false;
	}
	return true;
}

bool Projectile::collide(GameObject* obj)
{
	if (id == obj->getID()){
		return false;
	}
	float dist = (obj->getPos() - position).Mag();
	float dot = forward.Dot(obj->getPos() - position);
	if (obj->isPlayer){
		printf("PID = %d\n", ((Player*)obj)->getPID());
		printf("distance = %f\n", dist);
		printf("init pos = %f,%f,%f\n", initPos.x, initPos.y, initPos.z);
		printf("Position = %f,%f,%f\n",position.x,position.y,position.z);
		printf("Obj Position = %f,%f,%f\n", obj->getPos().x, obj->getPos().y, obj->getPos().z);
		printf("distance traveld = %f\n", distance);
	}
	if (dist <= hitRadius){
		
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