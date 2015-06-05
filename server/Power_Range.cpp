#include "stdafx.h"
#include "Power_Range.h"
#include "GameLogic.h"

Power_Range::Power_Range()
{
	ad = 30;
	c_ad = ad;
	maxcd = atof(ConfigSettings::config->getValue("PowerCD").c_str());
	disableTime = atof(ConfigSettings::config->getValue("PowerStunDuration").c_str());
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
		
		for (int i = 15; i > 0; i--){
			float spd = i;
			if (!(player->collide(&GameLogic::gameObjects, -1*player->forward,spd))){
				player->setPos(player->getPos() - player->forward *spd);
				std::string* change = new std::string("pos:");
				GameObject::changes.push_back(std::pair<int, std::string*>(player->getID(), change));
				break;
			}
		}
		player->isDisabled(disableTime);
		
	}

	createPowerProjectile(obj);
}