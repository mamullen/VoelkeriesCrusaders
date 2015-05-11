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
	void addEvent(unsigned int id, char * evt);
	Packet * popServerEvent();

private:
	std::vector<std::string> serverIPList;
	char user_input[PACKET_DATA_LEN];
	std::map<unsigned int,std::string *> inputEvents;
	std::vector<Packet *> serverEvents;
};



