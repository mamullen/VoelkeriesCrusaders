#include "stdafx.h"
#include "Projectile.h"
#include "GameLogic.h"

Projectile::Projectile()
{
	speed = atof(ConfigSettings::config->getValue("ProjectileSpeed").c_str());
	range = atof(ConfigSettings::config->getValue("ProjectileRange").c_str());
	modifier = 1;
	hitRadius = 6;
	isRecoil = true;
	recoilAmount = atof(ConfigSettings::config->getValue("ProjectileRecoil").c_str());
	fromTeam = 0;
}


Projectile::~Projectile()
{
}

bool Projectile::updateTime(int time, std::vector<GameObject*>* objects)
{
	if (collideWall(objects, time)){
		return false;
	}
	position = position + speed*time / 1000 * (forward.Normalize());
	distance += speed*time / 1000;
	sendPosPacket();
	int hit = 0;
	for (int i = 0; i < GameLogic::playerList.size(); i++){
		if (collide(GameLogic::playerList[i]) && fromTeam != GameLogic::playerList[i]->objectType){
			printf("target hit!\n");
			Player* p = GameLogic::playerList[i];
			p->isAttacked(dmg * modifier);
			if (isRecoil){
				forward.Normalize();
				float prevSpeed = p->getSpeed();
				for (int i = 0; i < recoilAmount*prevSpeed; i++){
					p->setSpeed(i);
					if (!(p->collide(objects, forward))){
						p->setPos(p->getPos() + forward*i);
					}
					else
						break;
				}
				p->setSpeed(prevSpeed);
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(p->getID(), change));
			}
			if (isStun){
				p->isDisabled(2000);
			}
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
		printf("Position = %f,%f,%f\n", position.x, position.y, position.z);
		printf("Obj Position = %f,%f,%f\n", obj->getPos().x, obj->getPos().y, obj->getPos().z);
		printf("distance traveld = %f\n", distance);
	}
	if (dist <= hitRadius){

		if (dot < 0){
			return false;
		}
		if (dot > 0.8){
			modifier = 1.1;
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

void Projectile::sendPosPacket(){
	float x = position.x;
	float y = position.y;
	float z = position.z;
	///////////////////////////////////////////////////////////////////////////
	char data[PACKET_DATA_LEN];
	int pointer = 0;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "p_pos", 6);
	pointer += 6;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float));
	pointer += sizeof(float);
	data[pointer] = ',';
	pointer++;
	///////////////////////////////////////////////////////////////////////////
	Packet* p = new Packet;
	p->packet_type = ACTION_EVENT;
	memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
	p->id = this->id;

	GameLogic::serverPackets.push_back(p);
}

void Projectile::sendDeathPacket()
{
	///////////////////////////////////////////////////////////////////////////
	char data[PACKET_DATA_LEN];
	int pointer = 0;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "p_die", 6);
	pointer += 6;
	data[pointer] = ',';
	pointer++;
	///////////////////////////////////////////////////////////////////////////
	Packet* p = new Packet;
	p->packet_type = ACTION_EVENT;
	memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
	p->id = this->id;

	GameLogic::serverPackets.push_back(p);
}

bool Projectile::collideWall(std::vector<GameObject*>* obj, int time)
{
	Vector3 objmin, objmax, playerposition;
	playerposition = (this->getPos()) + forward*speed*time / 1000;
	//printf("Number of objs %d\n\n", obj->size());
	for (int i = 0; i < obj->size(); i++)
	{

		objmin = obj->at(i)->getMin();
		objmax = obj->at(i)->getMax();



		if ((objmin.x <= playerposition.x && playerposition.x <= objmax.x))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
		{
			if ((objmin.z <= playerposition.z && playerposition.z <= objmax.z))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
			{
				return true;
			}
		}


	}
	return false;
}