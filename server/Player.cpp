#include "stdafx.h"
#include "Player.h"
#include <iostream>

Player::Player() :GameObject()
{
	// attributes
	position = Vector3(0, 1.3, 0);
	rotation = 0;
	hp = 100;
	speed = (float)atoi(ConfigSettings::config->getValue("PlayerMoveSpeed").c_str());
	// actions
	setAttack(new Basic_Attack());
	//////////////////////////////////
	isPlayer = true;

	pPosition = Vector3(position.x, position.y, position.z);
	forward = Vector3(0, 0, 1);
	pForward = Vector3(forward.x, forward.y, forward.z);
	right = Vector3(-1, 0, 0);
	up = Vector3(0, 1, 0);

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
}
Player::~Player()
{
}

void Player::update(Packet* packet, std::vector<GameObject*>* objects)
{
	std::string in = std::string(packet->packet_data);
	int pid = packet->id;
	int currInd = 0;
	int currEnd = in.find(';', currInd);
	std::cout << "packet update = " << in.c_str() << std::endl;

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
				this->moveForward();
			}
		}
		else if (cEvent.compare("move_backward") == 0){
			if (!isChanged[0]){
				change_counter[0]++;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				this->moveBackward();
			}
		}
		else if (cEvent.compare("move_left") == 0){
			if (!isChanged[0]){
				change_counter[0]++;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				this->strafeLeft();
			}
		}
		else if (cEvent.compare("move_right") == 0){
			if (!isChanged[0]){
				change_counter[0]++;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				this->strafeRight();
			}
		}
		else if (cEvent.substr(0,6).compare("rotate") == 0){

			std::string num = cEvent.substr(6, currEnd);
			num = num.substr(num.find_last_of(' ') + 1);
			int n = atoi(num.c_str());
			printf("%s\n", num.c_str());

			if (!isChanged[1]){
				change_counter[1]++;
				std::string* change = new std::string("rot:");
				changes.push_back(std::pair<int, std::string*>(id, change));
			}
			this->setRotation(n);
		}
		else if (cEvent.compare("attack") == 0){
			ad = attack_mode->getDmg();
			std::cout << "ATTACK!" << std::endl;
			std::cout << "AD = " << ad << std::endl;
			std::cout << "size = " << objects->size() << std::endl;
			if (attack_mode->getCD() <= 0){
				attack_mode->maxCD();
				for (int i = 0; i < objects->size(); i++)
				{
					std::cout << "Attacking id =  " << objects->at(i)->getID() << std::endl;
					if (objects->at(i)->isPlayer){
						std::cout << "CD =  " << attack_mode->getCD() << std::endl;
						this->attack(objects->at(i));
					}
				}
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

void Player::attack(GameObject* obj)
{

	if (this->id == obj->getID()){
		return;
	}
	if (this->inRange(obj)){
		obj->isAttacked(ad);
	}
}

void Player::isAttacked(float ad)
{
	if (!isChanged[2] && hp > 0){
		change_counter[2]++;
		std::string* change = new std::string("hp");
		changes.push_back(std::pair<int, std::string*>(id, change));
		hp -= ad;
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