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
	void setID(int theID);
	float getHP();

	void moveForward();
	void moveBackward();
	void setRotation(float f);
	void rotLeft();
	void rotRight();
	void strafeLeft();
	void strafeRight();

	
	virtual void isAttacked(float) = 0;
	virtual void update(Packet*, std::vector<GameObject*>*) = 0;

	std::map<std::string*, bool> getChanges();
	void addChange(std::string* change);
	void clearChanges();
	//void getStatusPacket();

	static std::list<std::pair<int, std::string*>> changes;
	bool isPlayer;
	int objectType; // 0 = building
	Vector3 forward;
protected:
	static unsigned int totalId;

	const float ROTATE_LEFT = 1;
	const float ROTATE_RIGHT = -1;
	const float PI = 3.14159;
	
	float hp;
	float rotation;
	Matrix34 rotateMx;
	Vector3 position, right, up;
	Vector3 pPosition, pForward;
	double speed=.5;
	unsigned int id;
	unsigned int attr_num;  // number of attributes
	//std::list<std::string*, bool> changeList;
	bool *isChanged;		// array indicating if a attribute such as position has been modified
	int *change_counter;    // array that counts the # of changes to an attribute
};

