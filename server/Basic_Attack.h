#pragma once
#include "Action.h"
#include "ConfigSettings.h"
class Basic_Attack :
	public Action
{
public:
	Basic_Attack();
	virtual ~Basic_Attack();

	virtual float getDmg();
	virtual float getRange();
	virtual void setRange(float);
	virtual void attack(GameObject*,GameObject*);
private:
	bool inRange(GameObject*,GameObject*);
};
