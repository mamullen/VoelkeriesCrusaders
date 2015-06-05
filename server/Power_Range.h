#pragma once
#include "Basic_Range.h"

class Power_Range :
	public Basic_Range
{
public:
	Power_Range();
	virtual ~Power_Range();

	virtual void attack(GameObject*);
};

