#include "stdafx.h"
#include "Score.h"


Score::Score()
{
	kill_num = 0;
	death_num = 0;
}


Score::~Score()
{
}

void Score::incKill()
{
	kill_num++;
}

void Score::incDeath()
{
	death_num++;
}