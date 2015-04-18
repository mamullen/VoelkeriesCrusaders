#include "stdafx.h"
#include "ServerGame.h"

unsigned int ServerGame::client_id;

ServerGame::ServerGame()
{
	// id's to assign clients for our table
	client_id = 0;
	// set up the server network to listen 
	network = new ServerNetwork();
}


ServerGame::~ServerGame()
{
}

void ServerGame::createServer()
{
	printf("setting up server\n");
	network->createServer();
}

void ServerGame::update()
{
	// reply for searching server
	network->respondToFind();

	// get new clients
	if (network->acceptNewClient(client_id))
	{
		printf("client %d has been connected to the server\n", client_id);

		client_id++;
	}

	receiveFromClients();
}

void ServerGame::receiveFromClients()
{
	Packet packet;

	// go through all clients
	std::map<unsigned int, SOCKET>::iterator iter;

	for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
	{
		// get data for that client
		int data_length = network->receiveData(iter->first, network_data);

		if (data_length <= 0)
		{
			//no data recieved
			continue;
		}

		int i = 0;
		while (i < (unsigned int)data_length)
		{
			packet.deserialize(&(network_data[i]));
			i += sizeof(Packet);

			switch (packet.packet_type) {

			case INIT_CONNECTION:

				printf("server received init packet from client %d\n",iter->first);
				
				sendActionPackets(iter->first);

				break;

			case ACTION_EVENT:

				printf("server received action event packet from client %d\n", iter->first);
				printf(packet.packet_data);
				printf("\n");

				sendActionPackets(iter->first);

				break;

			case COMMUNICATION:

				printf("server received communication packet from client %d\n", iter->first);
				printf(packet.packet_data);
				printf("\n");
				packet.id = iter->first;
				
				sendCommunicationPackets(packet);
				break;
			default:

				printf("error in packet types\n");

				break;
			}
		}
	}
}

void ServerGame::sendActionPackets(unsigned int id)
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = ACTION_EVENT;
	memcpy(packet.packet_data,"hello",PACKET_DATA_LEN);

	packet.serialize(packet_data);

	network->sendToOne(id,packet_data, packet_size);
}

void ServerGame::sendCommunicationPackets(Packet& packet)
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);
	network->sendToAll(packet_data, packet_size);
}

const char* ServerGame::getIP()
{
	return network->localIP;
}