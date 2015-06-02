#include "stdafx.h"
#include "Player.h"
#include <iostream>

Player::Player() :GameObject()
{
	// attributes
	name = "anonymous";
	team = 0;
	position = Vector3(0, 1.3, 0);
	rotation = 0;
	hp = 100;
	default_speed = (float)atoi(ConfigSettings::config->getValue("PlayerMoveSpeed").c_str());
	speed = default_speed;
	isAlive = true;
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

	attr_num = 3;
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
	std::string in = std::string(packet->packet_data);
	int pid = packet->id;
	int currInd = 0;
	int currEnd = in.find(';', currInd);
	//std::cout << "packet update = " << in.c_str() << std::endl;
	this->gravity();
	if (hp <= 0){
		return;
	}

	while (currEnd != std::string::npos){
		std::string cEvent = in.substr(currInd, currEnd - currInd);
		currInd = currEnd + 1;
		currEnd = in.find(';', currInd);
		
		if (cEvent.compare("move_forward") == 0){
			if (!isChanged[0]){
				change_counter[0]++;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				if (!(collide(objects,forward)))
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
				this->jump();
			}

		}
		else if (cEvent.compare("q") == 0){
			//

			if (shrinecollide(Vector3(-60, 0, -60), Vector3(60, 0, 60)))
			{
				if (!isChanged[3]){
					change_counter[3]++;
					std::string* change = new std::string("particles");
					changes.push_back(std::pair<int, std::string*>(id, change));
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
				if (!(collide(objects,-right)))
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
				if (!(collide(objects,right)))
				{
					this->strafeRight();
				}
				hDir++;
			}
		}
		else if (cEvent.substr(0,6).compare("rotate") == 0){

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
	if ( dot < 0.75){
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
	attack_mode->update();
}

void Player::updateTime(int time,int delta){
	// does nothing =[
}

bool Player::collide(std::vector<GameObject*>* obj, Vector3 dir)
{
	
	Vector3 objmin, objmax, playerposition;
	playerposition = (this->getPos())+dir*speed;
	//printf("Number of objs %d\n\n", obj->size());
	for (int i = 0; i < obj->size(); i++)
	{
		
		objmin = obj->at(i)->getMin();
		objmax = obj->at(i)->getMax();
		
		

		if ((objmin.x <= playerposition.x && playerposition.x <= objmax.x))// || min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
		{


			if ((objmin.z <= playerposition.z && playerposition.z <= objmax.z))// || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
			{

				if (obj->at(i)->objectType == 0)
				{
					
					return true;
				}

				
			}
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
bool Player::shrinecollide(Vector3 min, Vector3 max)
{
	if (min.x < position.x && position.x <max.x)
	{
		if (min.z < position.z && position.z < max.z)
		{

			return true;
		}
	}
	return false;
}