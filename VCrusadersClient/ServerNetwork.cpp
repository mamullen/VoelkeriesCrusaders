#include "stdafx.h"
#include "ServerNetwork.h"


ServerNetwork::ServerNetwork()
{
	// create WSADATA object
	WSADATA wsaData;

	// our sockets for the server
	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(1);
	}
}

ServerNetwork::~ServerNetwork()
{
}

void ServerNetwork::createServer()
{
	// address info for the server to listen to
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	////////////////////////////////////// UDP SOCKET //////////////////////////////////////
	UDPSocket = socket(AF_INET, SOCK_DGRAM, 0);

	char broadcast = '1';

	if (setsockopt(UDPSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
	{
		printf("Error in setting broadcast option\n");
		closesocket(UDPSocket);
		//return 0;
		exit(1);
	}
	struct sockaddr_in Recv_addr;
	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(PORT1);
	Recv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(UDPSocket, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) < 0)
	{
		printf("Error in BINDING!\n");
		closesocket(UDPSocket);
		//return 0;
		exit(1);
	}

	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;
	iResult = ioctlsocket(UDPSocket, FIONBIO, &iMode);
	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////

	// set address information
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	// get sockaddr_in to get ip///////////////////////////////////////////////////////////
	char host_name[256];				/* Name of the server */
	struct hostent *hp;					/* Information about this computer */

	gethostname(host_name, sizeof(host_name));
	hp = gethostbyname(host_name);

	struct in_addr addr;
	memcpy(&addr, hp->h_addr_list[0], sizeof(struct in_addr));
	//	std::cout << "Address " << 0 << ": " << inet_ntoa(addr) << endl;
	memcpy(&localIP, inet_ntoa(addr), sizeof(localIP));
	//////////////////////////////////////////////////////////////////////////////////////

	// Set the mode of the socket to be nonblocking
	//u_long iMode = 1;
	iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

	if (iResult == SOCKET_ERROR) {
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// no longer need address information
	freeaddrinfo(result);

	// start listening for new clients attempting to connect
	iResult = listen(ListenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}
}

// accept new connections
bool ServerNetwork::acceptNewClient(unsigned int & id)
{
	// if client waiting, accept the connection and save the socket
	ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket != INVALID_SOCKET)
	{
		//disable nagle on the client's socket
		char value = 1;
		setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

		// insert new client into session id table
		sessions.insert(pair<unsigned int, SOCKET>(id, ClientSocket));

		return true;
	}

	return false;
}

// receive incoming data
int ServerNetwork::receiveData(unsigned int client_id, char * recvbuf)
{
	if (sessions.find(client_id) != sessions.end())
	{
		SOCKET currentSocket = sessions[client_id];
		iResult = NetworkServices::receiveMessage(currentSocket, recvbuf, MAX_PACKET_SIZE);
		if (iResult == 0)
		{
			printf("Connection closed\n");
			closesocket(currentSocket);
		}
		return iResult;
	}
	return 0;
}

// send data to all clients
void ServerNetwork::sendToAll(char * packets, int totalSize)
{
	SOCKET currentSocket;
	std::map<unsigned int, SOCKET>::iterator iter;
	int iSendResult;

	for (iter = sessions.begin(); iter != sessions.end(); iter++)
	{
		currentSocket = iter->second;
		iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalSize);

		if (iSendResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(currentSocket);
		}
	}
}

void ServerNetwork::sendToOne(unsigned int id, char * packets, int totalSize)
{
	SOCKET currentSocket;
	currentSocket = sessions.at(id);
	std::map<unsigned int, SOCKET>::iterator iter;
	int iSendResult;
	iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalSize);

	if (iSendResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(currentSocket);
	}
}

int ServerNetwork::respondToFind()
{
	struct sockaddr_in Sender_addr;

	int len = sizeof(struct sockaddr_in);

	char recvbuff[50];
	int recvbufflen = 50;
	char sendMSG[] = "OK I'm the server";

	iResult = recvfrom(UDPSocket, recvbuff, recvbufflen, 0, (sockaddr*)&Sender_addr, &len);
	if (iResult > 0){
		printf("Server received message is ");
		printf(recvbuff);
		printf("\n");

		if (strcmp(recvbuff, "I'm looking for a server") == 0){
			for (int i = 0; i < 1; i++){
				iResult = sendto(UDPSocket, localIP, strlen(localIP) + 1, 0, (sockaddr *)&Sender_addr, sizeof(Sender_addr));
				Sleep(1000);
			}
		}
	}

	return 0;
}

