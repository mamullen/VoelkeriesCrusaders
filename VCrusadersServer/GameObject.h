#pragma once

#include "vector3.h"
#include "matrix34.h"
#include "NetworkData.h"
#include <map>
#include <list>
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
	float getRot();
	int getID();

	void moveForward();
	void moveBackward();
	void setRotation(float f);
	void rotLeft();
	void rotRight();
	void strafeLeft();
	void strafeRight();

	virtual void update(Packet*) = 0;

	std::map<std::string*, bool> getChanges();
	void addChange(std::string* change);
	void clearChanges();
	//void getStatusPacket();

	static std::list<std::pair<int, std::string*>> changes;

protected:
	static unsigned int totalId;

	const float ROTATE_LEFT = 1;
	const float ROTATE_RIGHT = -1;
	const float PI = 3.14159;

	float rotation;
	Matrix34 rotateMx;
	Vector3 position, forward, right, up;
	Vector3 pPosition, pForward;
	double speed=.5;
	unsigned int id;
	unsigned int attr_num;  // number of attributes
	//std::list<std::string*, bool> changeList;
	bool *isChanged;		// array indicating if a attribute such as position has been modified
};

