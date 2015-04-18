#include "stdafx.h"
#include "GamePlay.h"


GamePlay::GamePlay()
{
	isServer = false;
	client = new ClientGame();
	server = new ServerGame();
}

GamePlay::~GamePlay()
{
}

void GamePlay::printPrompt()
{
	printf("Welcome to Game!\n");
	printf("Press 1 for searching and viewing servers\n");
	printf("Press 2 for creating a server\n");
	printf("Press 3 to manually connect to a server with IP\n");
	printf("Type your message: ");
	std::cin.getline(user_input, PACKET_DATA_LEN);
	printf(user_input);
	printf("\n");
}

void GamePlay::processInput()
{
	if (strcmp(user_input, "1") == 0){
		client->network->findServer(serverIPList);
		printf("here is a list of servers we can choose from!\n");
		for (int i = 0; i < serverIPList.size(); i++){
			printf("Server %d at: ", i);
			printf(serverIPList[i]);
			printf("\n");
		}
		// right now only auto connects to first server
		if (serverIPList.size() > 0){
			printf("choose a server! \n");
			std::cin.getline(user_input, PACKET_DATA_LEN);
			int index = atoi(user_input);
			printf("index = %d\n", index);
			if (index >= 0 && index < serverIPList.size()){
				client->connectToServer(serverIPList[index]);
				printf("connection successful!\n");
			}
			else{
				printf("Index error, start over again!\n");
				start();
			}
		}
		else{
			printf("no server found\n");
			start();
		}
	}
	else if (strcmp(user_input, "2") == 0){
		server->createServer();
		client->connectToServer(server->getIP());
		isServer = true;
	}
	else if (strcmp(user_input, "3") == 0){
		printf("Type in IP:\n");
		std::cin.getline(user_input, PACKET_DATA_LEN);
		client->connectToServer(user_input);
	}
}

void GamePlay::update()
{

}

void GamePlay::start()
{
	printPrompt();
	processInput();
}

void GamePlay::serverLoop()
{
	server->update();
}

void GamePlay::clientLoop()
{
	client->update();
}