#pragma once

#include "Player.h"
#include "NetworkData.h"

class PacketParser
{
public:
	PacketParser();
	~PacketParser();

	void parse(char* input, Player* player);
	Packet* createPacket(std::map<std::string*, bool> changes, GameObject * object);
};

