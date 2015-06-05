#include "stdafx.h"
#include "DisableTimer.h"


DisableTimer::DisableTimer()
{
	isDisable = false;
	max = 10000;
}


DisableTimer::~DisableTimer()
{
}

void DisableTimer::disable()
{
	isDisable = true;
	time = max;
}

void DisableTimer::update(int t)
{
	if (time > 0){
		time -= t;
	}
	else{
		isDisable = false;
	}
}
bool DisableTimer::updates(int t)
{
	if (time > 0){
		time -= t;
		return true;
	}
	else{
		return false;
	}
}

bool DisableTimer::getDisable()
{
	return isDisable;
}