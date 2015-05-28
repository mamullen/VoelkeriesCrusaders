#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{
	time = 0;
}


Timer::~Timer()
{
}


int Timer::getState()
{
	// 1 for night , 0 for day, -1 for invalid
	if (time >= 30000 && time <= 90000){
		return 1;
	}
	if (time < 0 || time > 120000){
		return -1;
	}
	return 0;
}