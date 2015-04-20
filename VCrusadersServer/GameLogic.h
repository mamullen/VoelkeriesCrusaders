#pragma once

#include "GameObject.h"
#include "Player.h"
#include "NetworkData.h"
#include "PacketParser.h"
#include <list>
#include <map>
#include <vector>

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	void update();
	std::list<Packet*> getServerPackets();
	void savePacket(int id,char* p);
	void clearPackets();
	void addPlayer(int id);

private:
	std::vector<GameObject*> gameObjects;
	std::list<std::pair<int,char*>> packets;
	std::list<Packet*> serverPackets;
	std::map<int, Player*> players;
	PacketParser* packetParser;
};

