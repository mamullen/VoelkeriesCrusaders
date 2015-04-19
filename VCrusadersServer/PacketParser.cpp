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
	if (strcmp(input, "move_forward")==0){
		player->moveForward();
	}
}

Packet* PacketParser::createPacket(std::map<char*, bool> changes, GameObject * object)
{
	char data[PACKET_DATA_LEN];
	int pointer = 0;

	for (std::map<char*, bool>::iterator it = changes.begin(); it != changes.end(); it++)
	{
		char* key = it->first;
		if (strcmp(key, "pos"))
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
		}
		else if (strcmp(key, "dir"))
		{

		}
	}

	Packet* p = new Packet;
	p->packet_type = ACTION_EVENT;
	memcpy(p->packet_data, data, PACKET_DATA_LEN);
	p->id = object->getID();
	return p;
}