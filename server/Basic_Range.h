#pragma once
#include "Action.h"
class Basic_Range :
	public Action
{
public:
	Basic_Range();
	virtual ~Basic_Range();

	virtual float getDmg();
	virtual float getRange();
	virtual void setRange(float);
	virtual void attack(GameObject*,GameObject*);
};

