#pragma once
#include "GameObject.h"
class Shockwave : public GameObject
{
public:
	Shockwave(Vector3* min, Vector3* max) : GameObject(min, max, 500){ setMaxHealth(1); setHealth(1); setRotation(0); };
	void drawObj();
	void setAlpha(float a)		{ alpha = a; };
	void decreaseTime(unsigned int t);
	unsigned int getTime()	{ return time; }
private:
	float alpha = 1;
	unsigned int time = 1500;
};

