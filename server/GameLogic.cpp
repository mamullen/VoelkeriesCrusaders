#include "stdafx.h"
#include "GameLogic.h"
#include <stdio.h>
using namespace std;

vector<Projectile*> GameLogic::projectileList;
vector<Player*> GameLogic::playerList;
list<Packet*> GameLogic::serverPackets;

GameLogic::GameLogic()
{
	//packetParser = new PacketParser();
	crusadersToStart = atoi(ConfigSettings::config->getValue("CrusadersToStart").c_str());
	vampiresToStart = atoi(ConfigSettings::config->getValue("VampiresToStart").c_str());
	phase1time = atoi(ConfigSettings::config->getValue("Phase1Time").c_str());
	phase2time = atoi(ConfigSettings::config->getValue("Phase2Time").c_str());
	phase3time = atoi(ConfigSettings::config->getValue("Phase3Time").c_str());
	ticksPerTimerSend = atoi(ConfigSettings::config->getValue("TicksPerTimerSend").c_str());
	ticksSinceSend = ticksPerTimerSend;

	numCrusaders = 0;
	numVampires = 0;
	aliveCrusaders = 0;
	aliveVampires = 0;

	gameState = WAIT;
	timer = new Timer();
	timer->setPhase(phase1time, phase2time, phase3time);
}


GameLogic::~GameLogic()
{
	delete timer;
}

// update will do the following
// 1. updates the game object with the packet it intends to modify
// 2. create packets for clients when all packets received from clients are processed
void GameLogic::update(int time)
{

	updateState();

	/* TEMPORARY BREAK */
	if (getState() == WAIT)
		return;

	if (getState() == END){
		/*char data[PACKET_DATA_LEN];
		memcpy_s(data, PACKET_DATA_LEN, "game_over", 10);
		Packet* p = new Packet;
		p->packet_type = ACTION_EVENT;
		memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
		p->id = 0;
		serverPackets.push_back(p);*/
		return;
	}

	// update projectiles
	float p_update_freq = 15; // update frequency
	for (int i = 0; i < p_update_freq; i++){
		for (std::vector<Projectile*>::iterator it = GameLogic::projectileList.begin(); it != GameLogic::projectileList.end(); it++){
			if (!(*it)->updateTime(time/p_update_freq)){
				(*it)->sendDeathPacket();
				GameLogic::projectileList.erase(it);
				it--;
			}
		}
	}
	

	// go through each player and update their attack cd and status based on time
	for (int i = 0; i < gameObjects.size(); i++){
		if (gameObjects[i]->isPlayer){
			((Player *)gameObjects[i])->updateCD();
			((Player *)gameObjects[i])->updateTime(timer->getState(), time);
			((Player *)gameObjects[i])->resetDir();
		}
	}

	// go through packets and update according to its id
	for (std::list<std::pair<int, Packet*>>::const_iterator iter = packets.begin(); iter != packets.end(); ++iter){
		if (iter->second->id < gameObjects.size()){
			//printf("gamelogic update\n");
			gameObjects.at(iter->second->id)->update(iter->second, &gameObjects);
		}
	}

	// go through each player and check if their dir has changed
	for (int i = 0; i < gameObjects.size(); i++){
		if (gameObjects[i]->isPlayer){
			((Player *)gameObjects[i])->checkChangedDir();
		}
	}

	// go through GameObject changes, update each entry, and then create packets
	for (std::list<std::pair<int, std::string*>>::iterator it = GameObject::changes.begin(); it != GameObject::changes.end(); it++){
		//printf("gamelogic update 2\n");
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
		else if (key->compare("dead") == 0){
			float x = gameObjects.at(index)->getPos().x;
			float y = gameObjects.at(index)->getPos().y;
			float z = gameObjects.at(index)->getPos().z;
			///////////////////////////////////////////////////////////////////////////
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "dead", 5);
			pointer += 5;
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
		else if (key->compare("weapon1") == 0){
			float x = gameObjects.at(index)->getPos().x;
			float y = gameObjects.at(index)->getPos().y;
			float z = gameObjects.at(index)->getPos().z;
			///////////////////////////////////////////////////////////////////////////
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "weapon1", 8);
			pointer += 8;
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
		else if (key->compare("weapon2") == 0){
			float x = gameObjects.at(index)->getPos().x;
			float y = gameObjects.at(index)->getPos().y;
			float z = gameObjects.at(index)->getPos().z;
			///////////////////////////////////////////////////////////////////////////
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "weapon2", 8);
			pointer += 8;
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
		else if (key->compare("weapon3") == 0){
			float x = gameObjects.at(index)->getPos().x;
			float y = gameObjects.at(index)->getPos().y;
			float z = gameObjects.at(index)->getPos().z;
			///////////////////////////////////////////////////////////////////////////
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "weapon3", 8);
			pointer += 8;
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
		else if (key->compare("weapon4") == 0){
			float x = gameObjects.at(index)->getPos().x;
			float y = gameObjects.at(index)->getPos().y;
			float z = gameObjects.at(index)->getPos().z;
			///////////////////////////////////////////////////////////////////////////
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "weapon4", 8);
			pointer += 8;
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
		else if (key->compare("particles") == 0){
			float r = gameObjects.at(index)->getHP();
			///////////////////////////////////////////////////////////////////////////
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "particles", 10);
			pointer += 10;
			data[pointer] = ',';
			pointer++;
			///////////////////////////////////////////////////////////////////////////
			Packet* p = new Packet;
			p->packet_type = ACTION_EVENT;
			memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
			p->id = index;

			serverPackets.push_back(p);
		}
		else if (key->compare("hDir") == 0){
			if (gameObjects.at(index)->isPlayer){
				Player* player = (Player *)gameObjects.at(index);
				int hDir = player->getHDir();

				///////////////////////////////////////////////////////////////////////////
				char data[PACKET_DATA_LEN];
				int pointer = 0;
				memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "hdir", 5);
				pointer += 5;
				memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &hDir, sizeof(int));
				pointer += sizeof(int);
				data[pointer] = ',';
				pointer++;
				///////////////////////////////////////////////////////////////////////////
				Packet* p = new Packet;
				p->packet_type = ACTION_EVENT;
				memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
				p->id = index;

				serverPackets.push_back(p);
			}
		}
		else if (key->compare("vDir") == 0){
			if (gameObjects.at(index)->isPlayer){
				Player* player = (Player *)gameObjects.at(index);
				int vDir = player->getVDir();

				///////////////////////////////////////////////////////////////////////////
				char data[PACKET_DATA_LEN];
				int pointer = 0;
				memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "vdir", 5);
				pointer += 5;
				memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &vDir, sizeof(int));
				pointer += sizeof(int);
				data[pointer] = ',';
				pointer++;
				///////////////////////////////////////////////////////////////////////////
				Packet* p = new Packet;
				p->packet_type = ACTION_EVENT;
				memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
				p->id = index;

				serverPackets.push_back(p);
			}
		}
		// clear the change bool array after done processing all the changes 
		// need fix
		gameObjects.at(index)->clearChanges();
	}


	timer->update(time);
	if (ticksSinceSend == ticksPerTimerSend){
		ticksSinceSend = 0;
		//timer->print();

		int r = timer->getTime();
		char data[PACKET_DATA_LEN];
		int pointer = 0;
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "game_time", 10);
		pointer += 10;
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &r, sizeof(int));
		pointer += sizeof(int);
		data[pointer] = ',';
		pointer++;
		///////////////////////////////////////////////////////////////////////////
		Packet* p = new Packet;
		p->packet_type = ACTION_EVENT;
		memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
		p->id = 0;

		serverPackets.push_back(p);
	}
	ticksSinceSend++;


}

std::list<Packet*> GameLogic::getServerPackets()
{
	return serverPackets;
}

void GameLogic::pushServerPacket(Packet * p){
	serverPackets.push_back(p);
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

int GameLogic::addPlayer(int id, char* packet_data)
{

	bool playerExists = false;
	int playerExistsRet = 0;
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++){
		if ((*it)->getID() == id){
			playerExists = true;
			Player * currP = currP = (Player*)(*it);
			int currTeam = currP->team;
			if (!strcmp(packet_data, "choose_crusader;")){
				if (currTeam == 1)
					return -1;
				if (numCrusaders >= crusadersToStart){
					printf("Crusader team is full, not allowing client to join\n");
					playerExistsRet = 0;
				}
				else{
					numVampires--;
					numCrusaders++;
					Crusader * crus = new Crusader(id);
					crus->name = ((Player*)(*it))->name;
					it = gameObjects.erase(it);
					it = gameObjects.insert(it, crus);
					printf("PLAYER %d SWITCHED TEAMS\n", id);
					playerExistsRet = 1;
				}
			}
			else if (!strcmp(packet_data, "choose_vampire;")){
				if (currTeam == 2)
					return -1;
				if (numVampires >= vampiresToStart){
					printf("Vampire team is full, not allowing client to join\n");
					playerExistsRet = 0;
				}
				else{
					numCrusaders--;
					numVampires++;
					Vampire * vamp = new Vampire(id);
					vamp->name = ((Player*)(*it))->name;
					it = gameObjects.erase(it);
					it = gameObjects.insert(it, vamp);
					printf("PLAYER %d SWITCHED TEAMS\n", id);
					playerExistsRet = 2;
				}
			}
		}

	}

	if (playerExists)
		return playerExistsRet;

	int team = 0;

	printf("Attempting to add player %d with request: %s\n", id, packet_data);


	if (!strcmp(packet_data, "choose_crusader;")){
		printf("NUM CRUSADERS IS %d,, MAX IS %d\n", numCrusaders, crusadersToStart);
		if (numCrusaders >= crusadersToStart){
			printf("Crusader team is full, not allowing client to join\n");
			return 0;
		}
		numCrusaders++;
		team = 1;
	}
	else if (!strcmp(packet_data, "choose_vampire;")){
		if (numVampires >= vampiresToStart){
			printf("Vampire team is full, not allowing client to join\n");
			return 0;
		}
		numVampires++;
		team = 2;
	}

	Player* newP = 0;
	if (team == 1){
		newP = new Crusader(id);
	}
	else if (team == 2){
		newP = new Vampire(id);
	}
	// newP->team = team;
	for (std::list<std::pair<int, string>>::const_iterator nameIter = playerNames.begin(); nameIter != playerNames.end(); nameIter++){
		if (nameIter->first == id)
			newP->name = nameIter->second;
	}

	gameObjects.push_back(newP);
	GameLogic::playerList.push_back(newP);

	return team;

	//add shrine here?


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
		/*
		if ((*it)->objectType == 0){ // BUILDING... for now
		int id = (*it)->getID();
		Packet* p = new Packet;
		p->packet_type = ACTION_EVENT;
		p->id = id;
		//p->id = gameObjects.size() - 1;
		Building* building = (Building*)(*it);
		//printf("CREATING THIS BUILDING ID: %d\n", id);
		float minX = building->getMin().x;
		float minY = building->getMin().y;
		float minZ = building->getMin().z;
		float maxX = building->getMax().x;
		float maxY = building->getMax().y;
		float maxZ = building->getMax().z;
		float r = (*it)->getRot();
		char data[PACKET_DATA_LEN];
		int pointer = 0;
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "create_building", 16);
		pointer += 16;
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minX, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minY, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minZ, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxX, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxY, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxZ, sizeof(float));
		pointer += sizeof(float);
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &r, sizeof(float));
		pointer += sizeof(float);
		data[pointer] = ',';
		memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
		serverPackets.push_back(p);
		}
		else if ((*it)->objectType > 2 && (*it)->objectType < 5) { // 3: human, 4: crusader, 5: vampire
		*/
		int id = (*it)->getID();
		int objType = (*it)->objectType;
		Packet* p = new Packet;
		p->packet_type = ACTION_EVENT;
		p->id = id;


		if (objType == 0){
			Building* building = (Building*)(*it);
			float minX = building->getMin().x;
			float minY = building->getMin().y;
			float minZ = building->getMin().z;
			float maxX = building->getMax().x;
			float maxY = building->getMax().y;
			float maxZ = building->getMax().z;
			float r = (*it)->getRot();
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "create", 7);
			pointer += 7;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &objType, sizeof(int));
			pointer += sizeof(int);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minX, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minY, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minZ, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxX, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxY, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxZ, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &r, sizeof(float));
			pointer += sizeof(float);
			data[pointer] = ',';
			memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
			serverPackets.push_back(p);
		}
		else if (objType >= 3 && objType <= 5){
			float x = (*it)->getPos().x;
			float y = (*it)->getPos().y;
			float z = (*it)->getPos().z;
			float r = (*it)->getRot();
			float hp = (*it)->getHP();
			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "create", 7);
			pointer += 7;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &objType, sizeof(int));
			pointer += sizeof(int);
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
	int count = gameObjects.size();
	aliveCrusaders = 0;
	aliveVampires = 0;
	for (int i = 0; i < gameObjects.size(); i++){
		if (gameObjects[i]->isPlayer){
			if (gameObjects[i]->getHP() <= 0){
				count--;
			}
			else{
				int team = ((Player *)gameObjects[i])->team;
				if (team == 1)
					aliveCrusaders++;
				else if (team == 2)
					aliveVampires++;
			}
		}
	}

	if (gameState == WAIT)
	{
		if (numCrusaders >= crusadersToStart && numVampires >= vampiresToStart){
			printf("We have %d/%d crusaders and %d/%d vampires, STARTING THE GAME!\n", aliveCrusaders, crusadersToStart, aliveVampires, vampiresToStart);
			gameState = START;


			std::sort(gameObjects.begin(), gameObjects.end(), [](GameObject* a, GameObject* b){ return a->getID() < b->getID(); });


			char data[PACKET_DATA_LEN];
			memcpy_s(data, PACKET_DATA_LEN, "game_start", 11);
			Packet* p = new Packet;
			p->packet_type = JOIN_GAME;
			memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
			p->id = 0;
			serverPackets.push_back(p);
			printf("number of times\n");
			//MEMORY ISSUE #1 DEADPOOL
			Building * Build = new Building();
			/*Build->setMin(-180, -4, -160);
			Build->setMax(-150, 30, -110);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-150, -4, -160);
			Build->setMax(-120, 30, -110);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-120, -4, -160);
			Build->setMax(-110, 30, -110);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(155, -4, -80);
			Build->setMax(170, 25, -40);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(155, -4, 40);
			Build->setMax(170, 25, 80);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(140, -4, -80);
			Build->setMax(155, 25, -40);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(140, -4, -40);
			Build->setMax(155, 25, 0);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(140, -4, 0);
			Build->setMax(155, 25, 40);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(140, -4, 40);
			Build->setMax(155, 25, 80);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-155, -4, 40);
			Build->setMax(-140, 25, 80);
			gameObjects.push_back(Build);*/
			Build = new Building();
			Build->setMin(-240, -4, -240);
			Build->setMax(-200, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-200, -4, -240);
			Build->setMax(-160, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-160, -4, -240);
			Build->setMax(-120, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-120, -4, -240);
			Build->setMax(-80, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-80, -4, -240);
			Build->setMax(-40, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-40, -4, -240);
			Build->setMax(0, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(0, -4, -240);
			Build->setMax(40, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(40, -4, -240);
			Build->setMax(80, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(80, -4, -240);
			Build->setMax(120, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(120, -4, -240);
			Build->setMax(160, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(160, -4, -240);
			Build->setMax(200, 6, -225);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(200, -4, -240);
			Build->setMax(240, 6, -225);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-240, -4, 225);
			Build->setMax(-200, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-200, -4, 225);
			Build->setMax(-160, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-160, -4, 225);
			Build->setMax(-120, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-120, -4, 225);
			Build->setMax(-80, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-80, -4, 225);
			Build->setMax(-40, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-40, -4, 225);
			Build->setMax(0, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(0, -4, 225);
			Build->setMax(40, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(40, -4, 225);
			Build->setMax(80, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(80, -4, 225);
			Build->setMax(120, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(120, -4, 225);
			Build->setMax(160, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(160, -4, 225);
			Build->setMax(200, 6, 240);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(200, -4, 225);
			Build->setMax(240, 6, 240);
			gameObjects.push_back(Build);
			
			///
			Build = new Building();
			Build->setMin(225, -4, -240);
			Build->setMax(240, 6, -200);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, -200);
			Build->setMax(240, 6, -160);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, -160);
			Build->setMax(240, 6, -120);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, -120);
			Build->setMax(240, 6, -80);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, -80);
			Build->setMax(240, 6, -40);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, -40);
			Build->setMax(240, 6, 0);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, 0);
			Build->setMax(240, 6, 40);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, 40);
			Build->setMax(240, 6, 80);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, 80);
			Build->setMax(240, 6, 120);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, 120);
			Build->setMax(240, 6, 160);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, 160);
			Build->setMax(240, 6, 200);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(225, -4, 200);
			Build->setMax(240, 6, 240);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-240, -4, -240);
			Build->setMax(-225, 6, -200);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, -200);
			Build->setMax(-225, 6, -160);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, -160);
			Build->setMax(-225, 6, -120);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, -120);
			Build->setMax(-225, 6, -80);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, -80);
			Build->setMax(-225, 6, -40);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, -40);
			Build->setMax(-225, 6, 0);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, 0);
			Build->setMax(-225, 6, 40);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, 40);
			Build->setMax(-225, 6, 80);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, 80);
			Build->setMax(-225, 6, 120);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, 120);
			Build->setMax(-225, 6, 160);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, 160);
			Build->setMax(-225, 6, 200);
			gameObjects.push_back(Build);
			Build = new Building();
			Build->setMin(-240, -4, 200);
			Build->setMax(-225, 6, 240);
			gameObjects.push_back(Build);


			//BuildingsVVV

			Build = new Building();
			Build->setMin(-225, -4, -180);
			Build->setMax(-190, 9, -140);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-190, -4, -180);
			Build->setMax(-140, 9, -140);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-160, -4, -140);
			Build->setMax(-140, 11, -80);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-225, -4, -100);
			Build->setMax(-185, 11, -50);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-225, -4, -50);
			Build->setMax(-185, 11, 0);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-225, -4, 0);
			Build->setMax(-185, 11, 50);
			gameObjects.push_back(Build);

			
			Build = new Building();
			Build->setMin(-125, -4, 80);
			Build->setMax(-100, 11, 100);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-125, -4, 100);
			Build->setMax(-100, 11, 120);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-125, -4, 60);
			Build->setMax(-100, 11, 80);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-125, -4, 40);
			Build->setMax(-100, 11, 60);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-125, -4, 20);
			Build->setMax(-100, 11, 40);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-125, -4, 0);
			Build->setMax(-100, 11, 20);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-125, -4, -20);
			Build->setMax(-100, 11, 0);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-150, -4, 60);
			Build->setMax(-125, 11, 80);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-150, -4, 40);
			Build->setMax(-125, 11, 60);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-150, -4, 20);
			Build->setMax(-125, 11, 40);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-150, -4, 0);
			Build->setMax(-125, 11, 20);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-150, -4, -20);
			Build->setMax(-125, 11, 0);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-150, -4, 80);
			Build->setMax(-125, 11, 100);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-150, -4, 100);
			Build->setMax(-125, 11, 120);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-125, -4, 120);
			Build->setMax(-100, 11, 140);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-125, -4, 140);
			Build->setMax(-100, 11, 160);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-225, -4, 150);
			Build->setMax(-190, 11, 180);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-190, -4, 150);
			Build->setMax(-160, 11, 180);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-80, -4, 200);
			Build->setMax(-50, 15, 225);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-80, -4, 175);
			Build->setMax(-50, 15, 200);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(0, -4, 90);
			Build->setMax(30, 15, 105);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-30, -4, 90);
			Build->setMax(0, 15, 105);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(-30, -4, 160);
			Build->setMax(30, 15, 190);
			gameObjects.push_back(Build);


			Build = new Building();
			Build->setMin(-30, -4, 130);
			Build->setMax(-10, 15, 160);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(10, -4, 130);
			Build->setMax(30, 15, 160);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(50, -4, 100);
			Build->setMax(100, 15, 140);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(100, -4, 100);
			Build->setMax(130, 15, 140);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(100, -4, 140);
			Build->setMax(130, 15, 170);
			gameObjects.push_back(Build);

			Build = new Building();
			Build->setMin(130, -4, 140);
			Build->setMax(170, 15, 170);
			gameObjects.push_back(Build);

			// generating humans
			// Human* h = new Human();
			// gameObjects.push_back(h);


			//printf("PRINTING OUT THE GAMEOBJECTS VECTOR IDS\n");
			//for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++){
			//	printf("ObjectID: %d with type: %d\n", (*it)->getID(), (*it)->objectType);
			//}

			sendCreateObjects();
		}
	}
	else if (gameState == START)
	{

		if (crusadersToStart == 0 || vampiresToStart == 0){
			if (ticksSinceSend == ticksPerTimerSend)
				printf("Dev mode: 0 Crusaders or 0 Vampires to start selected NEVER ENEDING THE GAME...\n");
			return;
		}

		int gameover = 0; // 0: draw, 1: crusaders win, 2: vampires win
		if (aliveCrusaders == 0){
			printf("We have %d crusaders left, VAMPIRES WIN!\n", aliveCrusaders);
			gameState = END;
			gameover = 2;
		}
		else if (aliveVampires == 0){
			printf("We have %d vampires left, CRUSADERS WIN!\n", aliveCrusaders);
			gameState = END;
			gameover = 1;
		}
		else if (timer->getTime() >= phase3time){
			printf("Time is up. %d crusaders and %d vampires left. ", aliveCrusaders, aliveVampires);
			if (aliveCrusaders > aliveVampires){
				printf("CRUSADERS WIN!\n");
			}
			else if (aliveVampires > aliveCrusaders){
				printf("VAMPIRES WIN!\n");
			}
			else{
				printf("IT'S A DRAW!\n");
			}
			gameState = END;
			gameover = 0;
		}


		if (gameState == END){
			int pointer = 0;
			char data[PACKET_DATA_LEN];
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "game_over", 10);
			pointer += 10;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &gameover, sizeof(int));
			pointer += sizeof(int);
			data[pointer] = ',';
			pointer++;
			///////////////////////////////////////////////////////////////////////////
			Packet* p = new Packet;
			p->packet_type = ACTION_EVENT;
			memcpy_s(p->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
			p->id = 0;

			serverPackets.push_back(p);
		}
	}
}

