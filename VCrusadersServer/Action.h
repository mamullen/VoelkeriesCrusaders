#pragma once
#include "GameObject.h"

class Action
{
public:
	Action();
	virtual ~Action();

	virtual void update(GameObject&,GameObject&) = 0;
};

