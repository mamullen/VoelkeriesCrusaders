#include "stdafx.h"
#include "Player.h"
#include <iostream>

Player::Player() :GameObject()
{
	position = Vector3(0, 0, 0);
	pPosition = Vector3(position.x, position.y, position.z);
	forward = Vector3(0, 0, 1);
	pForward = Vector3(forward.x, forward.y, forward.z);
	right = Vector3(-1, 0, 0);
	up = Vector3(0, 1, 0);
	
	attr_num = 1;
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
				changes.insert(std::pair<int, std::string*>(id, change));
			}
			this->moveForward();
		}
		else if (cEvent.compare("move_backward") == 0){
			if (!isChanged[0]){
				isChanged[0] = true;
				std::string* change = new std::string("pos:");
				changes.insert(std::pair<int, std::string*>(id, change));
			}
			this->moveBackward();
		}
		else if (cEvent.compare("move_left") == 0){
			if (!isChanged[0]){
				isChanged[0] = true;
				std::string* change = new std::string("pos:");
				changes.insert(std::pair<int, std::string*>(id, change));
			}
			this->strafeLeft();
		}
		else if (cEvent.compare("move_right") == 0){
			if (!isChanged[0]){
				isChanged[0] = true;
				std::string* change = new std::string("pos:");
				changes.insert(std::pair<int, std::string*>(id, change));
			}
			this->strafeRight();
		}
	}
}

unsigned int Player::getPID()
{
	return pid;
}
