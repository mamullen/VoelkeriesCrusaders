#include "stdafx.h"
#include "GameLogic.h"


GameLogic::GameLogic()
{
}


GameLogic::~GameLogic()
{
}

void GameLogic::update()
{
	//Update Objects
	for (std::list<GameObject*>::const_iterator iter = gameObjects.begin(); iter != gameObjects.end(); ++iter){
		(**iter).update();
	}

	//Interpret packets
	for (std::list<Packet*>::const_iterator iter = packets.begin(); iter != packets.end(); ++iter){

	}

	clearPackets();
}

void GameLogic::savePacket(Packet p)
{
	packets.push_back(&p);
}

void GameLogic::clearPackets()
{
	packets.clear();
}