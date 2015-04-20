#pragma once
#include "ServerNetwork.h"
#include "NetworkData.h"
#include "GameLogic.h"

class ServerGame
{
public:
	ServerGame(GameLogic* g);
	virtual ~ServerGame();
	
	void createServer();
	void update();
	void receiveFromClients();
	void sendPackets();
	const char* getIP();

private:
	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;
	GameLogic* gameLogic;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

	void sendActionPackets(unsigned int);
	void sendCommunicationPackets(Packet&);
};

