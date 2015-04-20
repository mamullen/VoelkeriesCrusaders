#include "stdafx.h"
#include "ClientNetwork.h"


ClientNetwork::ClientNetwork()
{
	// create WSADATA object
	WSADATA wsaData;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(1);
	}

	//findServer();
	//connectToServer(localIP);
}

int ClientNetwork::receivePackets(char * recvbuf)
{
	iResult = NetworkServices::receiveMessage(ConnectSocket, recvbuf, MAX_PACKET_SIZE);

	if (iResult == 0)
	{
		printf("Connection closed\n");
		OutputDebugString("Connection closed\n");
		//char asdf[2000];
		//sprintf(asdf, "recv failed with error: %d\n", WSAGetLastError());
		//OutputDebugString(asdf);
		closesocket(ConnectSocket);
		WSACleanup();
		exit(1);
	}

	return iResult;
}


int ClientNetwork::findServer(std::vector<const char*>& serverList)
{
	// clear server list
	serverList.clear();

	// set up a UDP socket for broadcasting
	UDPSocket = socket(AF_INET, SOCK_DGRAM, 0);
	char broadcast = '1';

	if (setsockopt(UDPSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
	{
		printf("Error in setting Broadcast option\n");
		closesocket(UDPSocket);
		return 1;
	}

	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;
	iResult = ioctlsocket(UDPSocket, FIONBIO, &iMode);
	////////////////////////////////////////////////
	struct sockaddr_in Recv_addr;
	struct sockaddr_in Sender_addr;

	int len = sizeof(struct sockaddr_in);

	char recvbuff[50];
	int recvbufflen = 50;
	char sendMSG[] = "I'm looking for a server";

	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(PORT1);
	Recv_addr.sin_addr.s_addr = INADDR_BROADCAST;

	printf("start searching for a server!\n");

	for (int i = 0; i < 1; i++){
		iResult = sendto(UDPSocket, sendMSG, strlen(sendMSG) + 1, 0, (sockaddr *)&Recv_addr, sizeof(Recv_addr));
		Sleep(500);
	}

	do{
		iResult = recvfrom(UDPSocket, recvbuff, recvbufflen, 0, (sockaddr*)&Recv_addr, &len);
		if (iResult > 0){
			printf("message received is =>");
			printf(recvbuff);
			printf("\n");
			memcpy(localIP, recvbuff, sizeof(localIP));
			serverList.push_back(localIP);
			Sleep(500);
		}
	} while (iResult > 0);

	return 0;
}

int ClientNetwork::connectToServer(const char* ip)
{
	// socket
	ConnectSocket = INVALID_SOCKET;

	// holds address info for socket to connect to
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	// set address info
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

	//resolve server address and port 
	iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);

	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			printf("The server is down... did not connect");
		}
	}

	// no longer need address info for server
	freeaddrinfo(result);

	// if connection failed
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		exit(1);
	}

	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;

	iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		exit(1);
	}

	//disable nagle
	char value = 1;
	setsockopt(ConnectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

	return 1;
}

ClientNetwork::~ClientNetwork()
{
}
