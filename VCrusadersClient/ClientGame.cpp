#include "stdafx.h"
#include "ClientGame.h"
#include "numeric"


ClientGame::ClientGame()
{
	network = new ClientNetwork();

	/*

	*/
}


ClientGame::~ClientGame()
{
}

void ClientGame::sendActionPackets()
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[PACKET_DATA_LEN];
	char data[packet_size];

	Packet packet;
	packet.packet_type = ACTION_EVENT;
	if (!inputEvents.empty()){
		std::string acc = accumulate(inputEvents.begin(), inputEvents.end(), std::string(""));
		inputEvents.clear();
		char *accP = (char*)acc.c_str();
		memcpy(packet.packet_data, accP, sizeof(packet_data));

	}
	else{
		memcpy(packet.packet_data, ".", sizeof(packet_data));
	}

	packet.serialize(data);

	NetworkServices::sendMessage(network->ConnectSocket, data, packet_size);
}

void ClientGame::update()
{
	Packet packet;
	int data_length = network->receivePackets(network_data);

	if (data_length <= 0)
	{
		//no data recieved
		return;
	}

	int i = 0;
	while (i < (unsigned int)data_length)
	{
		packet.deserialize(&(network_data[i]));
		i += sizeof(Packet);
		
		switch (packet.packet_type) {

		case ACTION_EVENT:

			printf("client received action event packet from server\n");

			sendActionPackets();

			break;

		case COMMUNICATION:

			printf("client received communication packet from server\n");
			printf("Client %d said: ", packet.id);
			printf(packet.packet_data);
			printf("\n");

			sendActionPackets();
			break;
		default:

			printf("error in packet types\n");

			break;
		}
	}
}

void ClientGame::connectToServer(const char* ip)
{
	network->connectToServer(ip);

	// send init packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = INIT_CONNECTION;

	packet.serialize(packet_data);

	int result = NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
	printf("result of initialization = %d\n", result);
}