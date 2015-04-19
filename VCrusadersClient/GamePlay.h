#pragma once
#include <process.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "ClientGame.h"

class GamePlay
{
public:
	GamePlay();
	virtual ~GamePlay();
	void update();
	void start();
	bool isServer;
	void serverLoop();
	void clientLoop();
	void pushEvent(char * evt);

private:
	void printPrompt();
	void processInput();
	char user_input[PACKET_DATA_LEN];

	// attributes

	ClientGame *client;
	std::vector<const char*> serverIPList;
};

