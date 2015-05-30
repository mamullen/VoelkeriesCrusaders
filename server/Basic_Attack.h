#pragma once
#include "Action.h"
#include "ConfigSettings.h"
class Basic_Attack :
	public Action
{
public:
	Basic_Attack();
	virtual ~Basic_Attack();

	virtual void attack();
	virtual void attack(GameObject*,GameObject*);
	void setDotAngle(float n) { dotAngle = n; }
	void resetDotAngle(){ dotAngle = default_dotAngle; }
private:
	bool inRange(GameObject*,GameObject*);
	float dotAngle;
	float default_dotAngle;
};

