#include "stdafx.h"
#include "Basic_Range.h"


Basic_Range::Basic_Range()
{
}


Basic_Range::~Basic_Range()
{
}


float Basic_Range::getDmg()
{
	return c_ad;
}

float Basic_Range::getRange()
{
	return c_range;
}

void Basic_Range::setRange(float m){
	c_range = range * m;
}

void Basic_Range::attack(GameObject* obj,GameObject* target)
{

}