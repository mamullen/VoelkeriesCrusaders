#include "stdafx.h"
#include "Basic_Knock.h"
#include "GameLogic.h"
#include "GameObject.h"
#include "Player.h"


Basic_Knock::Basic_Knock()
{
	range = stof(ConfigSettings::config->getValue("MeleeKnockBackRange").c_str());
	c_range = range;
	maxcd = stof(ConfigSettings::config->getValue("MeleeKnockBackCD").c_str());
	ad = atof(ConfigSettings::config->getValue("MeleeKnockBackDMG").c_str());
	c_ad = ad;
}


Basic_Knock::~Basic_Knock()
{
}

void Basic_Knock::attack(GameObject* obj, GameObject* target)
{
	if (obj->getID() == target->getID()){
		return;
	}
	if (obj->objectType == target->objectType)
		return;
	if (inRange(obj, target)){
		if (target->objectType == 5 && target->getHP() >= 0){
			obj->addHp(atof(ConfigSettings::config->getValue("MeleeKnockBackLifesteal").c_str())*c_ad);
			target->removeHp(c_ad);
		}
		Vector3 direction = target->getPos() - obj->getPos();
		direction.Normalize();

		Player * player = (Player*)target;
		for (int i = 45; i > 0; i--){
			float spd = i;
			if (!(player->collide(&GameLogic::gameObjects, direction, spd))){
				player->setPos(player->getPos() + direction *spd);
				std::string* change = new std::string("pos:");
				GameObject::changes.push_back(std::pair<int, std::string*>(player->getID(), change));
				break;
			}
		}
		player->isDisabled(2000);
	}
}

bool Basic_Knock::inRange(GameObject* obj, GameObject* target)
{
	float distance = (target->getPos() - obj->getPos()).Mag();
	if (distance > getRange()){
		return false;
	}
	return true;
}

