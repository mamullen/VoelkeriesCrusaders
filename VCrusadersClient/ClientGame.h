#pragma once
#include "ClientNetwork.h"
#include "NetworkData.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

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
	std::vector<const char *> inputEvents;
	std::vector<const char *> serverEvents;

private:
	std::vector<std::string> serverIPList;
	char user_input[PACKET_DATA_LEN];
};



