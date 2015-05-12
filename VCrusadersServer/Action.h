#pragma once

class Action
{
public:
	Action();
	virtual ~Action();

	virtual float update() = 0;
};

