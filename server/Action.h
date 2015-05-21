#pragma once
class Action
{
public:
	Action();
	virtual ~Action();

	virtual float getDmg() = 0;
};

