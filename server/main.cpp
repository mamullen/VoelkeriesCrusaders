#include "stdafx.h"
#include "main.h"
// used for multi-threading
#include <process.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>

#define MS_PER_FRAME 50

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
	unsigned int lastTime =
		std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
	unsigned int currentTime = 0;
	unsigned int deltaTime = 0;

	while (true)
	{
		unsigned long long startTime =
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
		////////////////////////////////////////////////////////////////////
		// time for game logic
		currentTime =
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		////////////////////////////////////////////////////////////////////
		
		// server receive packets and accept connections
		server->update();

		// gameLogic updates and create packets for sending
		gameLogic->update(deltaTime);

		// server sends udpated packets
		server->sendPackets();

		// gameLogic resets, clear all packet buffer
		gameLogic->clearPackets();

		unsigned long long endTime =
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();

		//printf("wait time = %d\n", MS_PER_FRAME - (endTime-startTime));
		std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME-(endTime-startTime)));	
	}
	
}