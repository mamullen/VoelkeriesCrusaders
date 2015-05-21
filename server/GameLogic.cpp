#include "stdafx.h"
#include "GameLogic.h"

GameLogic::GameLogic()
{
	//packetParser = new PacketParser();
	crusadersToStart = atoi(ConfigSettings::config->getValue("CrusadersToStart").c_str());
	vampiresToStart = atoi(ConfigSettings::config->getValue("VampiresToStart").c_str());
	numCrusaders = 0;
	numVampires = 0;
	gameState = WAIT;
}


GameLogic::~GameLogic()
{
}

// update will do the following
// 1. updates the game object with the packet it intends to modify
// 2. create packets for clients when all packets received from clients are processed
void GameLogic::update()
{

	updateState();


	/* TEMPORARY BREAK */
	if (getState() == WAIT)
		return;

	if (getState() == END){
		char data[PACKET_DATA_LEN];
		memcpy_s(data, PACKET_DATA_LEN, "game_over", 10);
		Packet* p = new Packet;
		p->packet_type = ACTION_EVENT;
		memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
		p->id = 0;
		serverPackets.push_back(p);
		return;
	}

	// go through packets and update according to its id
	for (std::list<std::pair<int, Packet*>>::const_iterator iter = packets.begin(); iter != packets.end(); ++iter){
		if (iter->second->id < gameObjects.size()){
			printf("gamelogic update\n");
			gameObjects.at(iter->second->id)->update(iter->second, &gameObjects);
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
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "pos", 4);
			pointer += 4;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float));
			pointer += sizeof(float);
			data[pointer] = ',';
			pointer++;
			///////////////////////////////////////////////////////////////////////////
			Packet* p = new Packet;
			p->packet_type = ACTION_EVENT;
			memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
			p->id = index;

			serverPackets.push_back(p);
		}
		else if (key->compare("rot:") == 0){
			float r = gameObjects.at(index)->getRot();
			///////////////////////////////////////////////////////////////////////////
			//printf("rot = %f\n", r);
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "rot", 4);
			pointer += 4;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &r, sizeof(float));
			pointer += sizeof(float);
			data[pointer] = ',';
			pointer++;
			///////////////////////////////////////////////////////////////////////////
			Packet* p = new Packet;
			p->packet_type = ACTION_EVENT;
			memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
			p->id = index;

			serverPackets.push_back(p);
		}
		else if (key->compare("hp") == 0){
			float r = gameObjects.at(index)->getHP();
			///////////////////////////////////////////////////////////////////////////
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "hp", 3);
			pointer += 3;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &r, sizeof(float));
			pointer += sizeof(float);
			data[pointer] = ',';
			pointer++;
			///////////////////////////////////////////////////////////////////////////
			Packet* p = new Packet;
			p->packet_type = ACTION_EVENT;
			memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
			p->id = index;

			serverPackets.push_back(p);
		}

		// clear the change bool array after done processing all the changes 
		// need fix
		gameObjects.at(index)->clearChanges();
	}

	// go through each player and update their attack cd
	for (int i = 0; i < playerList.size(); i++){
		playerList[i]->updateCD();
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

void GameLogic::savePacket(int id, Packet* p)
{
	//printf("saved packet = ");
	//printf(p->packet_data);
	//printf("\n");
	packets.push_back(std::pair<int, Packet*>(id, p));
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

bool GameLogic::addPlayer(int id, char* packet_data)
{
	for (std::vector<Player*>::iterator it = playerList.begin(); it != playerList.end(); it++){
		if ((*it)->getPID() == id)
			return false;
	}

	printf("Attempting to add player %d with request: %s\n", id, packet_data);

	if (!strcmp(packet_data, "choose_crusader;")){
		if (numCrusaders >= crusadersToStart){
			printf("Crusader team is full, not allowing client to join\n");
			return false;
		}
		numCrusaders++;
	}
	else if (!strcmp(packet_data, "choose_vampire;")){
		if (numVampires >= vampiresToStart){
			printf("Vampire team is full, not allowing client to join\n");
			return false;
		}
		numVampires++;
	}

	Player* newP = new Player(id);
	gameObjects.push_back(newP);
	playerList.push_back(newP);
	//createNewObject(id);
	// createNewObjects();
	return true;


	printf("Added! We now have %d crusader(s) and %d vampire(s)\n", numCrusaders, numVampires);

}

void GameLogic::createNewObject(int id)
{

	Packet* p = new Packet;
	p->packet_type = ACTION_EVENT;
	p->id = id;
	//p->id = gameObjects.size() - 1;

	float x = gameObjects.at(p->id)->getPos().x;
	float y = gameObjects.at(p->id)->getPos().y;
	float z = gameObjects.at(p->id)->getPos().z;
	float r = gameObjects.at(p->id)->getRot();
	float hp = gameObjects.at(p->id)->getHP();
	char data[PACKET_DATA_LEN];
	int pointer = 0;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "new", 4);
	pointer += 4;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &r, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &hp, sizeof(float));
	pointer += sizeof(float);
	data[pointer] = ',';
	memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
	serverPackets.push_back(p);
}

void GameLogic::sendCreateObjects()
{

	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++){
		int id = (*it)->getID();
		Packet* p = new Packet;
		p->packet_type = ACTION_EVENT;
		p->id = id;
		//p->id = gameObjects.size() - 1;

		float x = (*it)->getPos().x;
		float y = (*it)->getPos().y;
		float z = (*it)->getPos().z;
		float r = (*it)->getRot();
		float hp = (*it)->getHP();
		char data[PACKET_DATA_LEN];
		int pointer = 0;
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "create", 7);
		pointer += 7;
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &r, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &hp, sizeof(float));
		pointer += sizeof(float);
		data[pointer] = ',';
		memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
		serverPackets.push_back(p);
	}
}

void GameLogic::setState(stateType state)
{
	gameState = state;
}

GameLogic::stateType GameLogic::getState()
{
	return gameState;
}

void GameLogic::updateState()
{
	int count = playerList.size();
	for (int i = 0; i < playerList.size(); i++){
		if (playerList[i]->getHP() <= 0){
			count--;
		}
	}

	if (gameState == WAIT)
	{
		if (numCrusaders >= crusadersToStart && numVampires >= vampiresToStart){
			printf("We have %d players, STARTING THE GAME!\n", count);
			gameState = START;

			char data[PACKET_DATA_LEN];
			memcpy_s(data, PACKET_DATA_LEN, "game_start", 11);
			Packet* p = new Packet;
			p->packet_type = JOIN_GAME;
			memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
			p->id = 0;
			serverPackets.push_back(p);

			sendCreateObjects();
		}
	}
	else if (gameState == START)
	{
		if (count <= 1){
			printf("We have %d player left, ENDING THE GAME!\n", 1);
			gameState == END;
		}
	}
}

/*
bool GameLogic::gameEnd()
{
if (!start){
return false;
}
int count = playerList.size();
for (int i = 0; i < playerList.size(); i++){
if (playerList[i]->getHP() <= 0){
count--;
}
}
if (count <= 1){
return true;
}
return false;
}
*/