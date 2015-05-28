#pragma once

#include "GameObject.h"
#include "Player.h"
#include "Vampire.h"
#include "Crusader.h"
#include "Building.h"
#include "NetworkData.h"
#include "PacketParser.h"
#include <list>
#include <map>
#include <vector>
#include <string>
#include "ConfigSettings.h"
#include <algorithm>
#include "Timer.h"
#include "Human.h"

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	void update(int time);
	std::list<Packet*> getServerPackets();
	void pushServerPacket(Packet *p);
	std::vector<GameObject*> getGameObjects();
	void savePacket(int id,Packet* p);
	void clearPackets();
	int addPlayer(int id, char* packet_data);
	void createNewObject(int id);
	void sendCreateObjects();
	enum stateType { WAIT, START, END };
	int crusadersToStart;
	int vampiresToStart;
	int numCrusaders;
	int numVampires;

	int aliveCrusaders;
	int aliveVampires;

	int phase1time;
	int phase2time;
	int phase3time;

	int ticksPerTimerSend;
	int ticksSinceSend;

	stateType getState();
	void setState(stateType state);
	void updateState();
	
	std::list<std::pair<int, string>> playerNames;

private:
	Timer* timer;
	std::vector<GameObject*> gameObjects;
	//std::vector<Player*> playerList;
	std::list<std::pair<int,Packet*>> packets;
	std::list<Packet*> serverPackets;
	std::map<int, Player*> players;
	PacketParser* packetParser;

	stateType gameState;
};

