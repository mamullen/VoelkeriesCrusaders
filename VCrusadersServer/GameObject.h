#pragma once

#include "vector3.h"
#include <map>

/*
 * Abstract base class for objects in the game
 */
class GameObject
{
public:
	GameObject();
	~GameObject();

	Vector3 getPos();
	int getID();

	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();

	virtual void update() = 0;

	std::map<std::string*, bool> getChanges();
	void addChange(std::string* change);
	void clearChanges();

private:
	static unsigned int totalId;

	Vector3 position, forward, right, up;
	Vector3 pPosition, pForward;
	double speed=.5;
	unsigned int id;
	std::map<std::string*, bool> changeList;
};

