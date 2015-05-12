#include "stdafx.h"
#include "NetworkServices.h"
#include <iostream>

NetworkServices::NetworkServices()
{
}


NetworkServices::~NetworkServices()
{
}

int NetworkServices::sendMessage(SOCKET curSocket, char * message, int messageSize)
{
	//return send(curSocket, message, messageSize, 0);
	
	int iResult;
	while (true){
		iResult = send(curSocket, message, messageSize, 0);
		if (iResult == SOCKET_ERROR && WSAGetLastError() == 10035){
			//printf("network service send failed with error: %d\n", WSAGetLastError());
			continue;
		}
		break;
	}
	return iResult;
}

int NetworkServices::receiveMessage(SOCKET curSocket, char * buffer, int bufSize)
{
	return recv(curSocket, buffer, bufSize, 0);
}