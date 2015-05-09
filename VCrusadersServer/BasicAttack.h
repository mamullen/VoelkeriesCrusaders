#pragma once
#include "Action.h"
class BasicAttack :
	public Action
{
public:
	BasicAttack();
	virtual ~BasicAttack();
	
	void update(GameObject&);
};

