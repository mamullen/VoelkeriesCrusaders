#pragma once
#include "Timer.h"
class DisableTimer :
	public Timer
{
public:
	DisableTimer();
	virtual ~DisableTimer();

	virtual void update(int t);
	virtual bool updates(int t);
	void disable();
	bool getDisable();
	void setsTime(float t) { time = t; }

private:
	bool isDisable;
};

