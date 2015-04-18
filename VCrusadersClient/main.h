#pragma once
#include "ServerGame.h"
#include "GameLogic.h"
#include "ClientGame.h"
#include "GamePlay.h"
class main
{
public:
	main();
	virtual ~main();
};



void serverLoop(void *);
void clientLoop(void);

ServerGame * server;
GameLogic * gameLogic;
ClientGame * client;
GamePlay * gameplay;
