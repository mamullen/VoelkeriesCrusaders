#include "stdafx.h"
#include "GameLogic.h"


GameLogic::GameLogic()
{
	packetParser = new PacketParser();
}


GameLogic::~GameLogic()
{
}

void GameLogic::update()
{
	//Update Objects
	for (unsigned int i = 0; i < gameObjects.size(); i++){
		gameObjects.at(i)->update();
	}

	//Interpret packets
	for (std::list<std::pair<int,char*>>::const_iterator iter = packets.begin(); iter != packets.end(); ++iter){
		char* input = iter->second;
		Player* player = players.at(iter->first);
		packetParser->parse(input,player);
	}

	//figure out what packets to send back to clients
	for (unsigned int i = 0; i < gameObjects.size(); i++){
		std::map<char*, bool> changes = gameObjects.at(i)->getChanges();
		if (changes.size()>0){
			serverPackets.push_back(packetParser->createPacket(changes, gameObjects.at(i)));
		}
		gameObjects.at(i)->clearChanges();
	}
}

std::list<Packet*> GameLogic::getServerPackets()
{
	return serverPackets;
}

void GameLogic::savePacket(int id,char* p)
{
	packets.push_back(std::pair<int,char*>(id,p));
}

void GameLogic::clearPackets()
{
	packets.clear();
	serverPackets.clear();
}

void GameLogic::addPlayer(int id)
{
	Player* newP = new Player(id);
	players.insert(std::pair<int, Player*>(id,newP));
	gameObjects.push_back(newP);
	
}