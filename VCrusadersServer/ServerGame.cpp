#include "stdafx.h"
#include "ServerGame.h"

unsigned int ServerGame::client_id;

ServerGame::ServerGame(GameLogic* g)
{
	// id's to assign clients for our table
	client_id = 0;
	// set up the server network to listen 
	network = new ServerNetwork();
	gameLogic = g;
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
	//network->respondToFind();

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
			Packet* temp = new Packet();
			temp->deserialize(&(network_data[i]));
			i += sizeof(Packet);

			switch (packet.packet_type) {

			case INIT_CONNECTION:

				printf("server received init packet from client %d\n",iter->first);
				sendInitPackets(iter->first);
				gameLogic->addPlayer(iter->first);
				sendActionPackets(iter->first);
				break;

			case ACTION_EVENT:
			{
				printf("server received action event packet from client %d\n", iter->first);
				printf(packet.packet_data);
				printf("\n");
				gameLogic->savePacket(iter->first, temp);
				//if (packet.id == 1000){
					sendActionPackets(iter->first);
				//}
				break;
			}
			case COMMUNICATION:

				printf("server received communication packet from client %d\n", iter->first);
				printf(packet.packet_data);
				printf("\n");
				//packet.id = iter->first;
				
				sendCommunicationPackets(packet);
				break;
			default:

				printf("error in packet types\n");

				break;
			}
		}
	}
}

void ServerGame::sendPackets()
{
	std::list<Packet*> serverPackets = gameLogic->getServerPackets();
	for (std::list<Packet*>::iterator it = serverPackets.begin(); it != serverPackets.end(); it++)
	{
		Packet p = **it;
		// send action packet
		printf("Packet Type: %d, Object ID: %d",p.packet_type, p.id);
		printf("\n");

		/*float test1;
		float test2;
		float test3;
		memcpy_s(&test1, sizeof(float), (p.packet_data) + 4, sizeof(float));
		memcpy_s(&test2, sizeof(float), (p.packet_data) + 8, sizeof(float));
		memcpy_s(&test3, sizeof(float), (p.packet_data) + 12, sizeof(float));

		printf("Coord: %f, %f, %f\n", test1, test2, test3);*/

		const unsigned int packet_size = sizeof(Packet);
		char packet_data[packet_size];

		p.serialize(packet_data);
		network->sendToAll(packet_data, packet_size);
	}
}

void ServerGame::sendInitPackets(unsigned int id)
{
	std::vector<GameObject*> gameObjects = gameLogic->getGameObjects();
	for (int i = 0; i < gameObjects.size(); i++){
		const unsigned int packet_size = sizeof(Packet);
		char packet_data[packet_size];
		Packet p;
		p.packet_type = ACTION_EVENT;
		p.id = i;
		memcpy_s(p.packet_data, PACKET_DATA_LEN, "create", 6);
		p.serialize(packet_data);
		network->sendToOne(id, packet_data, packet_size);
	}
}

void ServerGame::sendActionPackets(unsigned int id)
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = ACTION_EVENT;
	memcpy(packet.packet_data,"Hello",PACKET_DATA_LEN);

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