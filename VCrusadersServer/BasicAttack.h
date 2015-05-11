#pragma once
#include "Action.h"

class GameObject;

class BasicAttack :
	public Action
{
public:
	BasicAttack();
	virtual ~BasicAttack();
	
	virtual void update(GameObject*,GameObject*);
private:
	float getDis();
};

