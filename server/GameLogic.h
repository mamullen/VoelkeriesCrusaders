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
#include "Projectile.h"
#include "StealthBox.h"

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

	int crusScore;
	int vampScore;

	int phase1time;
	int phase2time;
	int phase3time;

	int ticksPerTimerSend;
	int ticksSinceSend;

	stateType getState();
	void setState(stateType state);
	void hardReset();
	void updateState();
	
	std::list<std::pair<int, string>> playerNames;
	static std::vector<Projectile*> projectileList;
	static std::vector<StealthBox*> stealthBoxList;
    static std::vector<Player*> playerList;
	static std::list<Packet*> serverPackets;

	static std::vector<GameObject*> gameObjects;
private:
	Timer* timer;
	
	std::list<std::pair<int,Packet*>> packets;
	
	std::map<int, Player*> players;
	PacketParser* packetParser;

	stateType gameState;
};

