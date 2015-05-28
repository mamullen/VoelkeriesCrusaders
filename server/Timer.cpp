#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{
	time = 0;
}


Timer::~Timer()
{
}

void Timer::setPhase(int a, int b, int c)
{
	p1 = a;
	p2 = b; 
	p3 = c;
}

int Timer::getState()
{
	// 1 for night , 0 for day, -1 for invalid
	if (time >= p1 && time <= p2){
		return 1;
	}
	if (time < 0 || time > p3){
		return -1;
	}
	return 0;
}