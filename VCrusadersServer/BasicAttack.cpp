#include "stdafx.h"
#include "BasicAttack.h"


BasicAttack::BasicAttack()
{
}


BasicAttack::~BasicAttack()
{
}

void BasicAttack::update(GameObject& self,GameObject& target){
	if (abs(self.getPos() - target.getPos().Mag) < self.attack_range){
		target.hp -= self.attack_dmg;
	}
}