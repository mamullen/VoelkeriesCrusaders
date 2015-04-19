#pragma once
#include "ServerNetwork.h"
#include "NetworkData.h"

class ServerGame
{
public:
	ServerGame();
	virtual ~ServerGame();
	
	void createServer();
	void update();
	void receiveFromClients();
	const char* getIP();

private:
	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];

	void sendActionPackets(unsigned int);
	void sendCommunicationPackets(Packet&);
};

