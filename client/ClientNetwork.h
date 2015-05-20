#pragma once
// Networking libraries
#include <winsock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include <ws2tcpip.h>
#include <stdio.h> 
#include <vector>
#include "NetworkData.h"
#include "ConfigSettings.h"

// size of our buffer
#define DEFAULT_BUFLEN 512
// port to connect sockets through 
#define DEFAULT_PORT ConfigSettings::config->getValue("ServerPort").c_str()
#define PORT1 6881
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ClientNetwork
{
public:
	ClientNetwork();
	virtual ~ClientNetwork();

	// for error checking function calls in Winsock library
	int iResult;

	// socket for client to connect to server
	SOCKET ConnectSocket;
	SOCKET UDPSocket;

	int connectToServer(const char* ip);  // connect to server with ip 
	int receivePackets(char *);
	int findServer(std::vector<const char*>&);
private:
	char localIP[INET_ADDRSTRLEN];
};

