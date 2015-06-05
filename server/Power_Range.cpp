#include "stdafx.h"
#include "Power_Range.h"
#include "GameLogic.h"

Power_Range::Power_Range()
{
	ad = 30;
	c_ad = ad;
	maxcd = 3.0;
}


Power_Range::~Power_Range()
{
}

void Power_Range::attack(GameObject* obj)
{
	if (getCD() > 0){
		return;
	}
	maxCD();
	if (obj->isPlayer){
		Player* player = (Player*)obj;
		player->isDisabled(500);
		player->setPos(player->getPos()-player->forward*10);
		std::string* change = new std::string("pos:");
		GameObject::changes.push_back(std::pair<int, std::string*>(player->getID(), change));
	}

	createPowerProjectile(obj);
}