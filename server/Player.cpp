#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "GameLogic.h"

Player::Player() :GameObject()
{
	controlShrine = false;
	// attributes
	name = "anonymous";
	team = 0;
	position = Vector3(0, 1.3, 0);
	rotation = 0;
	hp = 100;
	default_speed = atof(ConfigSettings::config->getValue("PlayerMoveSpeed").c_str());
	speed = default_speed;
	isAlive = true;
	// disable
	disableTimer.setMaxTime(500);
	// actions
	setAttack(new Basic_Attack());
	//////////////////////////////////
	time_ctr = 0;
	isPlayer = true;
	objectType = 2;
	pPosition = Vector3(position.x, position.y, position.z);
	forward = Vector3(0, 0, 1);
	up = Vector3(0, 1, 0);
	pForward = Vector3(forward.x, forward.y, forward.z);
	right = Vector3(-1, 0, 0);
	jumpup = Vector3(0, 1, 0);

	attr_num = 5;
	isChanged = new bool[attr_num];
	change_counter = new int[attr_num];
	for (int i = 0; i < attr_num; i++){
		isChanged[i] = false;
		change_counter[i] = 0;
	}
}

Player::Player(int i) :Player()
{
	pid = i;
	setID(i);
	controlShrine = false;

}
Player::~Player()
{
}

void Player::checkChangedDir(){
	if (vDir != vDirPrev){
		vDirPrev = vDir;
		std::string* change = new std::string("vDir");
		changes.push_back(std::pair<int, std::string*>(id, change));
	}
	if (hDir != hDirPrev){
		hDirPrev = hDir;
		std::string* change = new std::string("hDir");
		changes.push_back(std::pair<int, std::string*>(id, change));
	}
}

void Player::update(Packet* packet, std::vector<GameObject*>* objects)
{
	if (hp <= 0){
		return;
	}

	std::string in = std::string(packet->packet_data);

	//printf("STRING THING IS: %s\n", in.c_str());
	int pid = packet->id;
	int currInd = 0;
	int currEnd = in.find(';', currInd);

	Vector3 direction(0, 0, 0);
	//std::cout << "packet update = " << in.c_str() << std::endl;
	//this->gravity();

	while (currEnd != std::string::npos){
		std::string cEvent = in.substr(currInd, currEnd - currInd);
		currInd = currEnd + 1;
		currEnd = in.find(';', currInd);

		if (cEvent.compare("sw1") == 0){
			setAttack(default_attack_1);
		}
		else if (cEvent.compare("sw2") == 0){
			setAttack(default_attack_2);
		}
		if (disableTimer.getDisable()){
			continue;
		}
		if (cEvent.compare("move_forward") == 0){
			if (!isChanged[0]){
				change_counter[0]++;

				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				if (!(collide(objects, forward)))
				{
					this->moveForward();
				}
				vDir++;
			}

		}
		else if (cEvent.compare("move_jump") == 0){
			if (!isChanged[0]){
				change_counter[0]++;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				if (!(collide(objects, forward / 2)))
				{

					this->jump();
				}
			}

		}
		else if (cEvent.compare("q") == 0){
			//

			if (shrinecollide(Vector3(-35, 0, -35), Vector3(35, 0, 35)))
			{
				if (!isChanged[3]){
					change_counter[3]++;
					std::string* change = new std::string("particles");
					changes.push_back(std::pair<int, std::string*>(id, change));
					this->controlShrine = true;
					ShrineT(10);

				}
			}

		}
		else if (cEvent.compare("heal") == 0)
		{
			if (isAlive)
			{
				std::string* change = new std::string("hp");
				changes.push_back(std::pair<int, std::string*>(id, change));
				hp += 3;

			}
		}
		else if (cEvent.compare("b") == 0){
			if (weaponcollide(Vector3(-75, -1, -60)))
			{
				if (!isChanged[4]){
					change_counter[4]++;
					std::string* change = new std::string("weapon1");
					changes.push_back(std::pair<int, std::string*>(id, change));
					attack_mode->addDmg(2);
					this->setSpeed((7 * this->getSpeed()) / 8);
				}
			}
			if (weaponcollide(Vector3(-75, -1, 60)))
			{
				if (!isChanged[4]){
					change_counter[4]++;
					std::string* change = new std::string("weapon2");
					changes.push_back(std::pair<int, std::string*>(id, change));
					attack_mode->addDmg(2);
					this->setSpeed((7 * this->getSpeed()) / 8);
				}
			}
			if (weaponcollide(Vector3(75, -1, -60)))
			{
				if (!isChanged[4]){
					change_counter[4]++;
					std::string* change = new std::string("weapon3");
					changes.push_back(std::pair<int, std::string*>(id, change));
					attack_mode->addDmg(2);
					this->setSpeed((7 * this->getSpeed()) / 8);
				}
			}
			if (weaponcollide(Vector3(75, -1, 60)))
			{
				if (!isChanged[4]){
					change_counter[4]++;
					std::string* change = new std::string("weapon4");
					changes.push_back(std::pair<int, std::string*>(id, change));
					attack_mode->addDmg(2);
					this->setSpeed((2 * this->getSpeed()) / 3);
				}
			}
		}
		else if (cEvent.compare("move_backward") == 0){
			if (!isChanged[0]){
				change_counter[0]++;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				if (!(collide(objects, -forward)))
				{
					this->moveBackward();
				}
				vDir--;
			}
		}
		else if (cEvent.compare("move_left") == 0){
			if (!isChanged[0]){
				change_counter[0]++;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				if (!(collide(objects, -right)))
				{
					this->strafeLeft();
				}
				hDir--;
			}
		}
		else if (cEvent.compare("move_right") == 0){
			if (!isChanged[0]){
				change_counter[0]++;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				if (!(collide(objects, right)))
				{
					this->strafeRight();
				}
				hDir++;
			}
		}
		else if (cEvent.substr(0, 6).compare("rotate") == 0){

			std::string num = cEvent.substr(6, currEnd);
			num = num.substr(num.find_last_of(' ') + 1);
			int n = atoi(num.c_str());
			//printf("%s\n", num.c_str());

			if (!isChanged[1]){
				change_counter[1]++;
				std::string* change = new std::string("rot:");
				changes.push_back(std::pair<int, std::string*>(id, change));
			}
			this->setRotation(n);
		}
		else if (cEvent.compare("attack") == 0){
			//ad = attack_mode->getDmg();
			// melee attack
			if (objectType == 4)
				attack2EndExtra();
			if (attack_mode->getType() == 0){
				printf("melee!\n");
				if (attack_mode->getCD() <= 0){
					attack_mode->maxCD();
					for (int i = 0; i < objects->size(); i++)
					{
						if (objects->at(i)->isPlayer){
							attack_mode->attack(this, objects->at(i));
						}
					}
				}
			}
			// range attack
			else if (attack_mode->getType() == 1){
				//printf("range\n");
				attack_mode->attack(this);
			}
			else if (attack_mode->getType() == 2){ // basic knock shockwave business

				bool sendPacket = false;
				printf("melee!\n");
				if (attack_mode->getCD() <= 0){
					sendPacket = true;
					attack_mode->maxCD();
					for (int i = 0; i < objects->size(); i++)
					{
						if (objects->at(i)->isPlayer){
							attack_mode->attack(this, objects->at(i));
						}
					}
				}

				
				if (sendPacket){
					Packet* packet = new Packet;
					packet->packet_type = ACTION_EVENT;
					packet->id = getID();


					float x = getPos().x;
					float y = getPos().y;
					float z = getPos().z;
					float range = attack_mode->getRange();

					char data[PACKET_DATA_LEN];
					int pointer = 0;
					memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "shockwave", 10);
					pointer += 10;
					memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
					pointer += sizeof(float);
					memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
					pointer += sizeof(float);
					memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float));
					pointer += sizeof(float);
					memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &range, sizeof(float));
					pointer += sizeof(float);
					data[pointer] = ',';
					pointer++;
					memcpy_s(packet->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
					GameLogic::serverPackets.push_back(packet);
				}
			}
		}
		else if (cEvent.compare("attack2Start") == 0){
			attack2Start();
			//printf("ATTACK 2 START\n");
		}
		else if (cEvent.compare("attack2End") == 0){
			attack2End();
			//printf("ATTACK 2 END\n");

		}
	}

	for (int i = 0; i < attr_num; i++){
		if (change_counter[i] >0){
			isChanged[i] = true;
		}
	}
}

unsigned int Player::getPID()
{
	return pid;
}

//void Player::attack(GameObject* obj)
//{
//	if (this->id == obj->getID()){
//		return;
//	}
//	if (this->inRange(obj)){
//		obj->isAttacked(ad);
//	}
//}

void Player::isAttacked(float ad)
{
	if (hp > 0){
		change_counter[2]++;
		std::string* change = new std::string("hp");
		changes.push_back(std::pair<int, std::string*>(id, change));
		hp -= ad;
		if (hp <= 0){
			if (isAlive){
				std::string* change = new std::string("dead");
				changes.push_back(std::pair<int, std::string*>(id, change));
			}
			isAlive = false;
		}
	}
}

bool Player::inRange(GameObject* obj)
{
	float dot = (obj->getPos() - this->position).Normalize().Dot(this->forward.Normalize());
	float distance = (obj->getPos() - this->position).Mag();
	if (dot < 0.75){
		return false;
	}
	if (distance > attack_mode->getRange()){
		return false;
	}
	return true;
}

void Player::setAttack(Action* act)
{
	attack_mode = act;
}

void Player::updateCD()
{
	default_attack_1->update();
	default_attack_2->update();

	Packet* packet = new Packet;
	packet->packet_type = ACTION_EVENT;
	packet->id = getID();


	float cooldown1 = default_attack_1->getCD() / default_attack_1->getMaxCD();
	float cooldown2 = default_attack_2->getCD() / default_attack_2->getMaxCD();

	if (cooldown1 <= 0)
		cooldown1 = 0;

	if (cooldown2 <= 0)
		cooldown2 = 0;

	//printf("COOLDOWNS are 1: %f, and 2: %f \n", cooldown1, cooldown2);

	char data[PACKET_DATA_LEN];
	int pointer = 0;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "cooldown", 9);
	pointer += 9;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &cooldown1, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &cooldown2, sizeof(float));
	pointer += sizeof(float);
	data[pointer] = ',';
	pointer++;
	memcpy_s(packet->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
	GameLogic::serverPackets.push_back(packet);

}

void Player::updateTime(int time, int delta, std::vector<GameObject*>* obj){
	// does nothing =[
}

bool Player::collide(std::vector<GameObject*>* obj, Vector3 dir)
{
	ground = -1;
	Vector3 objmin, objmax, playerposition;
	playerposition = (this->getPos()) + dir*speed;
	//printf("Number of objs %d\n\n", obj->size());
	for (int i = 0; i < obj->size(); i++)
	{

		objmin = obj->at(i)->getMin();
		objmax = obj->at(i)->getMax();



		if ((objmin.x <= playerposition.x && playerposition.x <= objmax.x))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
		{
			if ((objmin.z <= playerposition.z && playerposition.z <= objmax.z))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
			{
				ground = objmax.y;
				if (playerposition.y > ground - 1)
				{
					return false;
				}
				if ((objmin.y <= playerposition.y && playerposition.y <= objmax.y))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
				{

					return true;


				}

			}
		}


	}
	if ((-32 <= playerposition.x && playerposition.x <= -23))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
	{
		if ((-32 <= playerposition.z && playerposition.z <= -23))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
		{
			return true;
		}
	}

	if ((23 <= playerposition.x && playerposition.x <= 32))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
	{
		if ((-32 <= playerposition.z && playerposition.z <= -23))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
		{
			return true;
		}
	}
	if ((23 <= playerposition.z && playerposition.z <= 32))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
	{
		if ((-32 <= playerposition.x && playerposition.x <= -23))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
		{
			return true;
		}
	}
	if ((23 <= playerposition.x && playerposition.x <= 32))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
	{
		if ((23 <= playerposition.z && playerposition.z <= 32))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
		{
			return true;
		}
	}

	if ((-15 <= playerposition.x && playerposition.x <= 15))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
	{
		if ((-15 <= playerposition.z && playerposition.z <= 15))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
		{
			return true;
		}
	}
	return false;
	//check to see is within world

	/*if ((-240 <= playerposition.x && playerposition.x <= 240))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
	{


	if ((-240 <= playerposition.z && playerposition.z <= 240))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
	{

	return false;
	}
	else
	{
	return true;
	}
	}
	else
	{
	return true;
	}*/
}

bool Player::collide(std::vector<GameObject*>* obj, Vector3 dir,float spd)
{
	ground = -1;
	Vector3 objmin, objmax, playerposition;
	playerposition = (this->getPos()) + dir*spd;
	//printf("Number of objs %d\n\n", obj->size());
	for (int i = 0; i < obj->size(); i++)
	{

		objmin = obj->at(i)->getMin();
		objmax = obj->at(i)->getMax();



		if ((objmin.x <= playerposition.x && playerposition.x <= objmax.x))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
		{
			if ((objmin.z <= playerposition.z && playerposition.z <= objmax.z))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
			{
				ground = objmax.y;
				if (playerposition.y > ground - 1)
				{
					return false;
				}
				if ((objmin.y <= playerposition.y && playerposition.y <= objmax.y))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
				{

					return true;


				}

			}
		}
	}
	return false;
}
bool Player::shrinecollide(Vector3 min, Vector3 max)
{
	if (min.x < position.x && position.x < max.x)
	{
		if (min.z < position.z && position.z < max.z)
		{

			return true;
		}
	}
	return false;
}

bool Player::weaponcollide(Vector3 weapon)
{
	Vector3 distance = position - weapon;
	float d = distance.Mag();
	if (d < 15) {
		return true;
	}
	return false;
}


void Player::respawnLocation(std::vector<GameObject*>* gameObjects){
	float mapX = atof(ConfigSettings::config->getValue("RespawnX").c_str());
	float mapZ = atof(ConfigSettings::config->getValue("RespawnZ").c_str());


	float randX = 0;
	float randY = 0;
	float randZ = 0;

	int spawningID = id;

	bool stillCollides = true;
	bool col1 = true;
	bool col2 = true;
	bool col3 = true;
	bool col4 = true;

	// randomness happening
	srand(spawningID*time(NULL));

	while (col1 || col2 || col3 || col4){

		int maxX = int(mapX * 2);
		int maxZ = int(mapZ * 2);
		randX = rand() % maxX - mapX;
		randZ = rand() % maxZ - mapZ;

		setPos(randX, randY, randZ);

		printf("Trying respawn position %f %f %f\n", randX, randY, randZ);


		col1 = collide(gameObjects, forward);
		col2 = collide(gameObjects, -forward);
		col3 = collide(gameObjects, right);
		col4 = collide(gameObjects, -right);

	}
}

void Player::respawn(std::vector<GameObject*>* gameObjects){
	respawnLocation(gameObjects);
	addHp(100);
	std::string* change = new std::string("pos:");
	changes.push_back(std::pair<int, std::string*>(id, change));
}

void Player::updateDisable(int time)
{
	disableTimer.update(time);
	shrineTimer.update(time);
}

void Player::isDisabled(int time)
{
	disableTimer.setMaxTime(time);
	disableTimer.disable();
}
void Player::ShrineT(int time)
{
	shrineTimer.setsTime(time);
	//shrineTimer.disable();
}
bool Player::getST()
{

}

bool Player::getDisabled()
{
	return disableTimer.getDisable();
}

bool Player::stillShrine(int time)
{
	return shrineTimer.updates(time);
}

