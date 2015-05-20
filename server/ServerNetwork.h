#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include <ws2tcpip.h>
#include <map>
#include <iostream>
#include "NetworkData.h"
#include "ConfigSettings.h"

using namespace std;
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881" 
#define PORT1 atoi(ConfigSettings::config->getValue("ServerPort").c_str())

class ServerNetwork
{
public:
	ServerNetwork();
	virtual ~ServerNetwork();

	// Socket to listen for new connections
	SOCKET ListenSocket;

	// Socket to give to the clients
	SOCKET ClientSocket;

	// Socket for listening to UDP broadcast
	SOCKET UDPSocket;

	// for error checking return values
	int iResult;

	// table to keep track of each client's socket
	std::map<unsigned int, SOCKET> sessions;

	// local machine's ip
	char localIP[INET_ADDRSTRLEN];

	// accept new connections
	bool acceptNewClient(unsigned int & id);

	// receive incoming data
	int receiveData(unsigned int client_id, char * recvbuf);

	// send data to all clients
	void sendToAll(char * packets, int totalSize);

	// send data to one client
	void sendToOne(unsigned int id, char * packets, int totalSize);
	// respond to client UDP broadcast
	int respondToFind();

	void createServer();
};

