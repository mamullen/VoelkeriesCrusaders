#include "stdafx.h"
#include "Human.h"


Human::Human() :Player()
{
	hp = 40;
	isPlayer = false;
	objectType = 3;
	position = Vector3(10, 10, 10);
	rotation = 0;
}


Human::~Human()
{
}
