#include "stdafx.h"
#include "PacketParser.h"


PacketParser::PacketParser()
{
}


PacketParser::~PacketParser()
{
}

void PacketParser::parse(char* input, Player* player)
{
	std::string in = std::string(input);
	int currInd = 0;

	//map used to check for duplicate client events (so player doesn't move too far in one update, etc.)
	std::map<std::string, bool> duplicates;

	int currEnd = in.find(';', currInd);
	while (currEnd != std::string::npos)
	{
		std::string cEvent = in.substr(currInd, currEnd - currInd);
		currInd = currEnd + 1;
		currEnd = in.find(';', currInd);

		if (cEvent == "move_forward" && duplicates.find("move_forward")==duplicates.end()){
			player->moveForward();
			duplicates.insert(std::pair<std::string, bool>(std::string("move_forward"), true));
		}
		if (cEvent == "move_backward" && duplicates.find("move_backward")==duplicates.end()){
			player->moveBackward();
			duplicates.insert(std::pair<std::string, bool>(std::string("move_backward"), true));
		}
		if (cEvent == "move_left" && duplicates.find("move_left") == duplicates.end()){
			player->strafeLeft();
			duplicates.insert(std::pair<std::string, bool>(std::string("move_left"), true));
		}
		if (cEvent == "move_right" && duplicates.find("move_right") == duplicates.end()){
			player->strafeRight();
			duplicates.insert(std::pair<std::string, bool>(std::string("move_right"), true));
		}
	}
}

Packet* PacketParser::createPacket(std::map<std::string*, bool> changes, GameObject * object)
{
	char data[PACKET_DATA_LEN];
	int pointer = 0;

	for (std::map<std::string*, bool>::iterator it = changes.begin(); it != changes.end(); it++)
	{
		std::string* key = it->first;
		if (*key == "pos")
		{
			float x = object->getPos().x;
			float y = object->getPos().y;
			float z = object->getPos().z;
			memcpy_s(data+pointer,PACKET_DATA_LEN-pointer,"pos:",4);
			pointer += 4;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float));
			pointer += sizeof(float);
			data[pointer] = ',';
			pointer++;
		}
		else if (*key == "dir")
		{
			
		}
	}

	//add '\0' to denote end of this packet
	data[pointer-1] = '\0';

	Packet* p = new Packet;
	p->packet_type = ACTION_EVENT;
	memcpy(p->packet_data, data, PACKET_DATA_LEN);
	p->id = object->getID();
	return p;
}