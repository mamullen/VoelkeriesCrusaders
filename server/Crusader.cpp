#include "stdafx.h"
#include "Crusader.h"


Crusader::Crusader()
{
}


Crusader::~Crusader()
{
}

Crusader::Crusader(int i) :Player(i)
{
}

void Crusader::updateTime(int time, int delta)
{
	if (time == 0){
		//day
		attack_mode->resetRange();
	}
	else if (time == 1){
		//night
		attack_mode->setRange(0.8);
	}
}