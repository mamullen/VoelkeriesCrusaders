#include "stdafx.h"
#include "ClientGame.h"
#include "numeric"


ClientGame::ClientGame()
{
	network = new ClientNetwork();
}


ClientGame::~ClientGame()
{
}

void ClientGame::sendActionPackets()
{
	// send action packets

	//only send if there are events to send to server
	std::map<unsigned int, std::string *>::iterator it;
	for (it = inputEvents.begin(); it != inputEvents.end(); it++)
	{
		const unsigned int packet_size = sizeof(Packet);
		char packet_data[PACKET_DATA_LEN];
		char data[packet_size];

		Packet packet;
		packet.packet_type = ACTION_EVENT;

		//printf("%s\n", (it->second)->c_str());

		memcpy(packet.packet_data, it->second, sizeof(packet_data));
		packet.id = it->first;
		packet.serialize(data);

		NetworkServices::sendMessage(network->ConnectSocket, data, packet_size);
	}
	inputEvents.clear();
}


void ClientGame::processActionPacket(char* the_data){
	char* token = strtok(the_data, ",");
	while (token != NULL){
		int dataSize = 0;

		if (strcmp(token, "pos:") == 0){
			dataSize = 17;
		}

		if (strcmp(token, "new") == 0){
			dataSize = 3;
		}

		if (strcmp(token, "create") == 0){
			dataSize = 6;
		}
				
		if (dataSize > 0){
			char * pToken = new char[dataSize];
			memcpy(pToken, token, dataSize * sizeof(char));
			//serverEvents.push_back(pToken);
		}
		token = strtok(NULL, ",");
	}

}

void ClientGame::update()
{
	//send packets that need to be sent to server
	sendActionPackets();

	//check what packets have been received
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
		Packet* packet = new Packet;
		packet->deserialize(&(network_data[i]));
		i += sizeof(Packet);

		serverEvents.push_back(packet);

		switch (packet->packet_type) {

		case ACTION_EVENT:

			//printf("client received action event packet from server\n");
			//processActionPacket(packet->packet_data);
			//printf(packet->packet_data);
			//printf("\n");
			

			break;

		case COMMUNICATION:

			printf("client received communication packet from server\n");
			printf("Client %d said: ", packet->id);
			printf(packet->packet_data);
			printf("\n");
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

void ClientGame::addEvent(unsigned int id, char * evt)
{
	//check if an event for this id has already been added
	if (inputEvents.find(id) == inputEvents.end()){ //not added before
		inputEvents.insert(std::pair<unsigned int, std::string *>(id, new std::string(evt)));
	}
	else{
		std::string* currEvt = inputEvents[id];
		currEvt->append(evt);
	}
}

Packet * ClientGame::popServerEvent(){
	if (!serverEvents.empty()){
		Packet * ret = serverEvents.back();
		serverEvents.pop_back();
		return ret;
	}
	return NULL;
}