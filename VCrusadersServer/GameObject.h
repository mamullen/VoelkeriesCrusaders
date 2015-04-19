#pragma once

#include "vector3.h"
#include <map>

/*
 * Abstract base class for objects in the game
 */
class GameObject
{
public:
	GameObject(int i);
	~GameObject();

	Vector3 getPos();
	int getID();

	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();

	virtual void update() = 0;

	std::map<char*, bool> getChanges();
	void addChange(char* change);
	void clearChanges();
private:
	Vector3 position, forward, right, up;
	Vector3 pPosition, pForward;
	double speed=.5;
	int id;
	std::map<char*, bool> changeList;
};

