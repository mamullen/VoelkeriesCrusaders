#include "stdafx.h"
#include "GameLogic.h"

GameLogic::GameLogic()
{
	//packetParser = new PacketParser();
}


GameLogic::~GameLogic()
{
}

// update will do the following
// 1. updates the game object with the packet it intends to modify
// 2. create packets for clients when all packets received from clients are processed
void GameLogic::update()
{
	// go through packets and update according to its id
	for (std::list<std::pair<int,Packet*>>::const_iterator iter = packets.begin(); iter != packets.end(); ++iter){
		if (iter->second->id < gameObjects.size()){
			printf("gamelogic update\n");
			gameObjects.at(iter->second->id)->update(iter->second);
		}
	}
	
	// go through GameObject changes, update each entry, and then create packets
	for (std::list<std::pair<int, std::string*>>::iterator it = GameObject::changes.begin(); it != GameObject::changes.end(); it++){
		printf("gamelogic update 2\n");
		std::string* key = it->second;
		int index = it->first;

		// create a position packet
		if (key->compare("pos:") == 0){
			float x = gameObjects.at(index)->getPos().x;
			float y = gameObjects.at(index)->getPos().y;
			float z = gameObjects.at(index)->getPos().z;

			///////////////////////////////////////////////////////////////////////////
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "pos:", 4);
			pointer += 4;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float)+1);
			pointer += sizeof(float);
			data[pointer] = ',';
			pointer++;
			///////////////////////////////////////////////////////////////////////////
			Packet* p = new Packet;
			p->packet_type = ACTION_EVENT;
			memcpy(p->packet_data, data, PACKET_DATA_LEN);
			p->id = index;

			serverPackets.push_back(p);
		}

		// clear the change bool array after done processing all the changes 
		// need fix
		gameObjects.at(index)->clearChanges();
	}
}

std::list<Packet*> GameLogic::getServerPackets()
{
	return serverPackets;
}

std::vector<GameObject*> GameLogic::getGameObjects()
{
	return gameObjects;
}

void GameLogic::savePacket(int id,Packet* p)
{
	//printf("saved packet = ");
	//printf(p->packet_data);
	//printf("\n");
	packets.push_back(std::pair<int,Packet*>(id,p));
}

void GameLogic::clearPackets()
{
	for (std::list<std::pair<int, Packet*>>::iterator it = packets.begin(); it != packets.end(); it++){
		delete it->second;
	}
	for (std::list<Packet*>::iterator it = serverPackets.begin(); it != serverPackets.end(); it++){
		delete *it;
	}
	for (std::list<std::pair<int, std::string*>>::iterator it = GameObject::changes.begin(); it != GameObject::changes.end(); it++){
		delete it->second;
	}

	packets.clear();
	serverPackets.clear();
	GameObject::changes.clear();
}

void GameLogic::addPlayer(int id)
{
	Player* newP = new Player(id);
	gameObjects.push_back(newP);
}

void GameLogic::createNewObjects()
{
	Packet* p = new Packet;
	p->packet_type = ACTION_EVENT;
	p->id = gameObjects.size()-1;
	memcpy_s(p->packet_data, PACKET_DATA_LEN, "new", 3+1);
	serverPackets.push_back(p);
}