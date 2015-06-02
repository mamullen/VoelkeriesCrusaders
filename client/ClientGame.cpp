#include "stdafx.h"
#include "ClientGame.h"
#include "numeric"


ClientGame::ClientGame()
{
	network = new ClientNetwork();
	stateChange = NULL;
}


ClientGame::~ClientGame()
{
}

void ClientGame::sendActionPackets()
{
	// send action packets

	//only send if there are events to send to server
	std::vector<std::tuple<unsigned int, int, std::string *>>::iterator it;
	for (it = inputEvents.begin(); it != inputEvents.end(); it++)
	{
		std::tuple<unsigned int, int, std::string *> tup = *it;

		const unsigned int packet_size = sizeof(Packet);
		//char packet_data[PACKET_DATA_LEN];
		char data[packet_size];

		Packet packet;
		packet.packet_type = std::get<1>(tup);

		//printf("%s\n", (it->second)->c_str());
		//printf("SIZE IS SOMETHING HERE %d\n", (it->second)->size());

		memcpy(packet.packet_data, std::get<2>(tup)->c_str(), std::get<2>(tup)->size() + 1);
		packet.id = std::get<0>(tup);
		packet.serialize(data);

		NetworkServices::sendMessage(network->ConnectSocket, data, packet_size);
	}
	inputEvents.clear();
}

void ClientGame::update()
{
	//send packets that need to be sent to server
	sendActionPackets();

	//check what packets have been received
	int data_length = network->receivePackets(network_data);

	if (data_length <= 0)
	{
		//no data recieved
		return;
	}
	//going to start at back so that we can keep server events in order
	int i = 0;// data_length - sizeof(Packet);
	while (i < data_length){
		i += sizeof(Packet);
	}
	i -= sizeof(Packet);

	//printf("%d\n", data_length);

	//printf("%d\n", i);
	while (i >= 0)
	{
		Packet* packet = new Packet;
		packet->deserialize(network_data+i);
		i -= sizeof(Packet);

		//printf(packet->packet_data);
		//printf("\n");

		switch (packet->packet_type) {

		case ACTION_EVENT:

			serverEvents.push_back(packet);
			break;

		case COMMUNICATION:

			printf("client received communication packet from server\n");
			printf("Client %d said: ", packet->id);
			printf(packet->packet_data);
			printf("\n");
			break;

		case JOIN_GAME:
			joinGameEvents.push_back(packet);
			break;

		default:

			//char* c = new char[sizeof(Packet)];
			//packet->serialize(c);
			printf("error in packet types\n\n");
			//std::cout.write(c,sizeof(Packet));
			//printf("\n\n");

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

void ClientGame::addEvent(unsigned int id, char * evt, int type)
{
	//check if an event for this id and type has already been added
	bool found = false;
	for (std::vector<std::tuple<unsigned int, int, std::string *>>::iterator it = inputEvents.begin(); it != inputEvents.end(); it++){
		std::tuple<unsigned int, int, std::string *> temp = *it;
		if (std::get<0>(temp) == id && std::get<1>(temp) == type){
			found = true;
			std::string * currEvt = std::get<2>(temp);
			currEvt->append(evt);
			it = inputEvents.end(); //end loop
			it--;
		}
	}
	if (!found){ //not added before
		inputEvents.push_back(std::tuple<unsigned int, int, std::string *>(id, type, new std::string(evt)));
	}
}

void ClientGame::addPlayer(unsigned int id, int team, char* name)
{
	std::map<unsigned int, std::pair<int, char*>>::iterator it = otherPlayers.find(id);
	if (it == otherPlayers.end()){
		//doesn't exist yet
		otherPlayers.insert(std::make_pair(id,std::make_pair(team,name)));
	}
	else{
		//already exists
		it->second.first = team;
		it->second.second = name;
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

Packet * ClientGame::popJoinGameEvent(){
	if (!joinGameEvents.empty()){
		Packet * ret = joinGameEvents.back();
		joinGameEvents.pop_back();
		return ret;
	}
	return NULL;
}