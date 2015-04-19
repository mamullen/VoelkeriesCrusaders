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
	gameLogic = new GameLogic();
	server = new ServerGame(gameLogic);
	
	server->createServer();
	// create thread with arbitrary argument for the run function
	serverLoop((void *)12);
}

//should most likely move this loop into another class... ServerThread class?
void serverLoop(void * arg)
{
	while (true)
	{
		unsigned long long startTime =
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();

		server->update();
		gameLogic->update();
		server->sendPackets();
		gameLogic->clearPackets();
		unsigned long long endTime =
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();

		//printf("%d\n", MS_PER_FRAME - (endTime-startTime));
		std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME-(endTime-startTime)));
		
	}
	
}