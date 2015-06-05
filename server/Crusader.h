#pragma once
#include "Player.h"
#include "Power_Range.h"
class Crusader :
	public Player
{
public:
	Crusader();
	Crusader(int i);
	virtual ~Crusader();
	virtual void updateTime(int, int, std::vector<GameObject*>*);
	void attack2Start() {};
	void attack2End() {};
	//virtual void attack(GameObject*);
};

