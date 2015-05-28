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
	void jump();

	void setMin(float, float, float);
	void setMax(float, float, float);
	void setMin(Vector3&);
	void setMax(Vector3&);
	Vector3 getMin();
	Vector3 getMax();

	
	virtual void isAttacked(float) = 0;
	virtual void update(Packet*, std::vector<GameObject*>*) = 0;

	std::map<std::string*, bool> getChanges();
	void addChange(std::string* change);
	void clearChanges();
	//void getStatusPacket();

	static std::list<std::pair<int, std::string*>> changes;
	bool isPlayer;
	int objectType; // 0: building, 2: player , 3:human, 4: vampire, 5: crusader
	Vector3 forward;
	Vector3 jumpup;
	void setType(int t){ objectType = t; }
protected:
	static unsigned int totalId;

	const float ROTATE_LEFT = 1;
	const float ROTATE_RIGHT = -1;
	const float PI = 3.14159;
	
	Vector3 min, max;

	float hp;
	float rotation;
	Matrix34 rotateMx;
	Vector3 position, right, up;
	Vector3 pPosition, pForward;
	double default_speed;
	double speed;
	unsigned int id;
	unsigned int attr_num;  // number of attributes
	//std::list<std::string*, bool> changeList;
	bool *isChanged;		// array indicating if a attribute such as position has been modified
	int *change_counter;    // array that counts the # of changes to an attribute
};

