#pragma once

#include "GameObject.h"
#include "Player.h"
#include "NetworkData.h"
#include "PacketParser.h"
#include <list>
#include <map>
#include <vector>
#include <string>

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	void update();
	std::list<Packet*> getServerPackets();
	std::vector<GameObject*> getGameObjects();
	void savePacket(int id,Packet* p);
	void clearPackets();
	void addPlayer(int id);
	void createNewObjects();

private:
	std::vector<GameObject*> gameObjects;
	std::list<std::pair<int,Packet*>> packets;
	std::list<Packet*> serverPackets;
	std::map<int, Player*> players;
	PacketParser* packetParser;
};

