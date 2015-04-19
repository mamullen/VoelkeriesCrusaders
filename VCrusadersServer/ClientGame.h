#pragma once
#include "ClientNetwork.h"
#include "NetworkData.h"
#include <iostream>
#include <vector>

class ClientGame
{
public:
	ClientGame();
	virtual ~ClientGame();

	ClientNetwork* network;
	char network_data[MAX_PACKET_SIZE];
	
	void sendActionPackets();
	void update();
	void connectToServer(const char*);

private:
	std::vector<const char*> serverIPList;
	char user_input[PACKET_DATA_LEN];
};

