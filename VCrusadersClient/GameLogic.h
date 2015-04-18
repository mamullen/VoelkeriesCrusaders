#pragma once

#include "GameObject.h"
#include "Player.h"
#include "NetworkData.h"
#include <list>
#include <map>

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	void update();
	void savePacket(Packet p);
	void clearPackets();

private:
	std::list<GameObject*> gameObjects;
	std::list<Packet*> packets;
	std::map<int, Player*> players;
};

