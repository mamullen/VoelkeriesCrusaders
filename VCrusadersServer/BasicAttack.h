#pragma once
#include "Action.h"

class BasicAttack :
	public Action
{
public:
	BasicAttack();
	virtual ~BasicAttack();
	
	virtual float update();
private:
	float getDis();
};

