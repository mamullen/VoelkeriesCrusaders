#pragma once

/*
 * Abstract base class for objects in the game
 */
class GameObject
{
public:
	GameObject(int i) : id(i){};
	~GameObject();

	double getX();
	double getY();
	double getZ();
	int getID();

	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();

	virtual void update()=0;
private:
	double xPos=0, yPos=0, zPos=0;
	double speed=1;
	int id;
};

