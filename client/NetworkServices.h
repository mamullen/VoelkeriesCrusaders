#pragma once
#include <winsock2.h>
#include <Windows.h>

class NetworkServices
{
public:
	NetworkServices();
	virtual ~NetworkServices();

	static int sendMessage(SOCKET curSocket, char * message, int messageSize);
	static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);
};

