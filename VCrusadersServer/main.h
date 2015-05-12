#pragma once
#include "ServerGame.h"
#include "GameLogic.h"
class main
{
public:
	main();
	virtual ~main();
};



void serverLoop(void *);

ServerGame * server;
GameLogic * gameLogic;
