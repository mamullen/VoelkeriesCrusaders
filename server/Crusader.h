#pragma once
#include "Player.h"
#include "StealthBox.h"
#include "GameLogic.h"
#include "Power_Range.h"
class Crusader :
	public Player
{
public:
	Crusader();
	Crusader(int i);
	virtual ~Crusader();
	virtual void updateTime(int, int, std::vector<GameObject*>*);
	void attack2Start();
	void attack2End() {};
	void attack2EndExtra();
	//virtual void attack(GameObject*);
	bool stealth = false;
	bool attack2started = false;

	float crusSpeed;

	StealthBox *sbox;
};

