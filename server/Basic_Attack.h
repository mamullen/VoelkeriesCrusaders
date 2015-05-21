#pragma once
#include "Action.h"
class Basic_Attack :
	public Action
{
public:
	Basic_Attack();
	virtual ~Basic_Attack();

	virtual float getDmg();
};

