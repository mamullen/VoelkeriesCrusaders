#pragma once
#include "Player.h"
#include "StealthBox.h"
#include "GameLogic.h"
class Crusader :
	public Player
{
public:
	Crusader();
	Crusader(int i);
	virtual ~Crusader();
	virtual void updateTime(int, int, std::vector<GameObject*>*);
	void attack2Start();
	void attack2End();
	//virtual void attack(GameObject*);
	bool stealth = false;

	StealthBox *sbox;
};

