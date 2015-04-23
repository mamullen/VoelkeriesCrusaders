#pragma once

#include "vector3.h"
#include "NetworkData.h"
#include <map>
#include <vector>
#include <iostream>

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

	virtual void update(Packet*) = 0;

	std::map<std::string*, bool> getChanges();
	void addChange(std::string* change);
	void clearChanges();

	static std::map<int,std::string*> changes;

protected:
	static unsigned int totalId;

	Vector3 position, forward, right, up;
	Vector3 pPosition, pForward;
	double speed=.5;
	unsigned int id;
	unsigned int attr_num;  // number of attributes
	std::map<std::string*, bool> changeList;
	bool *isChanged;		// array indicating if a attribute such as position has been modified
};

