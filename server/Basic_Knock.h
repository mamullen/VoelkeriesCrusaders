#pragma once
#include "Basic_Attack.h"
class Basic_Knock :
	public Basic_Attack
{
public:
	Basic_Knock();
	virtual ~Basic_Knock();

	virtual void attack(GameObject*, GameObject*);
protected:
	virtual bool inRange(GameObject*, GameObject*);
};

