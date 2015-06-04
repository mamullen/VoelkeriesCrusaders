#pragma once
#include "Player.h"
class Crusader :
	public Player
{
public:
	Crusader();
	Crusader(int i);
	virtual ~Crusader();
	virtual void updateTime(int,int);
	void attack2Start() {};
	void attack2End() {};
	//virtual void attack(GameObject*);
};

