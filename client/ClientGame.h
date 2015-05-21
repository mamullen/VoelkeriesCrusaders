#pragma once
#include "ClientNetwork.h"
#include "NetworkData.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include "ConfigSettings.h"

class ClientGame
{
public:
	ClientGame();
	virtual ~ClientGame();

	ClientNetwork* network;
	char network_data[MAX_PACKET_SIZE];
	
	void sendActionPackets();
	void processActionPacket(char*);
	void update();
	void connectToServer(const char*);
	void addEvent(unsigned int id, char * evt, int type);
	unsigned int getClientId()			{ return client_id; };
	void setClientId(unsigned int id)	{ client_id = id; }
	Packet * popServerEvent();
	Packet * popJoinGameEvent();
	void setStateChange(char* nextState) { stateChange = nextState; }
	char * getStateChange()				{ return stateChange; }

private:
	std::vector<std::string> serverIPList;
	char user_input[PACKET_DATA_LEN];
	std::vector<std::tuple<unsigned int, int, std::string *>> inputEvents;
	std::vector<Packet *> serverEvents;
	std::vector<Packet *> joinGameEvents;
	unsigned int client_id;
	char * stateChange;
};



