#pragma once
#include "Action.h"
#include "Player.h"
class BasicAttack :
	public Action
{
public:
	BasicAttack();
	virtual ~BasicAttack();
	
	void update(GameObject&,GameObject&);
private:
	float getDis();
};

