#include "stdafx.h"
#include "main.h"
// used for multi-threading
#include <process.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>

#define MS_PER_FRAME 200

main::main()
{
}


main::~main()
{
}

int main()
{
	/*
	// initialize the server
	server = new ServerGame();
	gameLogic = new GameLogic();

	// create thread with arbitrary argument for the run function
	//_beginthread(serverLoop, 0, (void*)12);
	serverLoop((void*)12);

	// initialize the client 
	//client = new ClientGame();

	//std::cin.get();
	clientLoop();
	*/
	gameplay = new GamePlay();
	gameplay->start();

	if (gameplay->isServer){
		printf("im server\n");
		// create thread with arbitrary argument for the run function
		_beginthread(serverLoop, 0, (void*)12);
}

	clientLoop();

}

//should most likely move this loop into another class... ServerThread class?
void serverLoop(void * arg)
{
	/*while (true)
	{
		unsigned long long startTime =
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();

		server->update();
		gameLogic->update();

		unsigned long long endTime =
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();

		//printf("%d\n", MS_PER_FRAME - (endTime-startTime));
		std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME-(endTime-startTime)));
		
	}*/
	while (true){
		gameplay->serverLoop();
	}
	
}

void clientLoop()
{
	while (true)
	{
		//do game stuff
		//will later run client->update();
		gameplay->clientLoop();
	}
}