#include "stdafx.h"
#include "Shrine.h"
#include <iostream>

Shrine::Shrine() :GameObject()
{
	// attributes
	objectType = 1;
	position = Vector3(0, 0, 0);
	
	
	
}

Shrine::Shrine(int i) :Shrine()
{
	pid = i;
	setID(i);

}
Shrine::~Shrine()
{
}

void Shrine::update(Packet* packet, std::vector<GameObject*>* objects)
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

		
		}
	
}

unsigned int Shrine::getPID()
{
	return pid;
}







