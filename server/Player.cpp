#include "stdafx.h"
#include "Player.h"
#include <iostream>

Player::Player() :GameObject()
{
	// attributes
	position = Vector3(0, 0, 0);
	rotation = 0;
	hp = 100;
	attack_dmg = 10;
	attack_range = 150;
	
	// actions

	//////////////////////////////////
	pPosition = Vector3(position.x, position.y, position.z);
	forward = Vector3(0, 0, 1);
	pForward = Vector3(forward.x, forward.y, forward.z);
	right = Vector3(-1, 0, 0);
	up = Vector3(0, 1, 0);


	attr_num = 5;
	isChanged = new bool[attr_num];
	for (int i = 0; i < attr_num; i++){
		isChanged[i] = false;
	}
}

Player::Player(int i) :Player()
{
	pid = i;
}
Player::~Player()
{
}

void Player::update(Packet* packet)
{
	std::string in = std::string(packet->packet_data);
	int pid = packet->id;
	int currInd = 0;
	int currEnd = in.find(';', currInd);
	std::cout << "packet update = " << in.c_str() << std::endl;
	while (currEnd != std::string::npos){
		std::string cEvent = in.substr(currInd, currEnd - currInd);
		currInd = currEnd + 1;
		currEnd = in.find(';', currInd);
		
		if (cEvent.compare("move_forward") == 0){
			if (!isChanged[0]){
				isChanged[0] = true;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				this->moveForward();
			}
		}
		else if (cEvent.compare("move_backward") == 0){
			if (!isChanged[0]){
				isChanged[0] = true;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				this->moveBackward();
			}
		}
		else if (cEvent.compare("move_left") == 0){
			if (!isChanged[0]){
				isChanged[0] = true;
				std::string* change = new std::string("pos:");
				changes.push_back(std::pair<int, std::string*>(id, change));
				this->strafeLeft();
			}
		}
		else if (cEvent.compare("move_right") == 0){
			if (!isChanged[0]){
				isChanged[0] = true;
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
				isChanged[1] = true;
				std::string* change = new std::string("rot:");
				changes.push_back(std::pair<int, std::string*>(id, change));
			}
			this->setRotation(n);
		}
		else if (cEvent.compare("basic_attack") == 0){

		}
		/*
		else if (cEvent.compare("rotate_left") == 0){
			if (!isChanged[1]){
				isChanged[1] = true;
				std::string* change = new std::string("rot:");
				changes.push_back(std::pair<int, std::string*>(id, change));
			}
			this->rotLeft();
		}
		else if (cEvent.compare("rotate_right") == 0){
			if (!isChanged[1]){
				isChanged[1] = true;
				std::string* change = new std::string("rot:");
				changes.push_back(std::pair<int, std::string*>(id, change));
			}
			this->rotRight();
		}
		*/
	}
}

unsigned int Player::getPID()
{
	return pid;
}

void Player::basic_attack(GameObject*)
{

}