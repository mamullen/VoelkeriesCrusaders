#pragma once

class GameObject;

class Action
{
public:
	Action();
	virtual ~Action();

	virtual void update(GameObject*,GameObject*) = 0;
};

