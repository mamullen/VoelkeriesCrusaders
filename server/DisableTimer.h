#pragma once
#include "Timer.h"
class DisableTimer :
	public Timer
{
public:
	DisableTimer();
	virtual ~DisableTimer();

	virtual void update(int t);
	void disable();
	bool getDisable();

private:
	bool isDisable;
};

