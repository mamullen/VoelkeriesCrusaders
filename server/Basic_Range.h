#pragma once
#include "Action.h"
#include "Projectile.h"

class Basic_Range :
	public Action
{
public:
	Basic_Range();
	virtual ~Basic_Range();

	virtual void attack(GameObject*);
};

