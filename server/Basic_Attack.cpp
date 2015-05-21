#include "stdafx.h"
#include "Basic_Attack.h"


Basic_Attack::Basic_Attack()
{
	cd = 0;
}


Basic_Attack::~Basic_Attack()
{
}

float Basic_Attack::getDmg()
{
	return (float)atoi(ConfigSettings::config->getValue("PlayerAttackDMG").c_str());
}

float Basic_Attack::getRange()
{
	return (float)atoi(ConfigSettings::config->getValue("PlayerAttackRange").c_str());
}

void Basic_Attack::maxCD()
{
	cd = (float)atoi(ConfigSettings::config->getValue("PlayerAttackCD").c_str());
}