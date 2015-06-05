#pragma once
#include "Player.h"
class Human :
	public Player
{
public:
	Human();
	virtual ~Human();
	void attack2Start() {};
	void attack2End() {};
	void attack2EndExtra() {};
};

