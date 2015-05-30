#pragma once
#include "GameObject.h"

class Projectile :
	public GameObject
{
public:
	Projectile();
	virtual ~Projectile();

	bool updateTime(int time);
	void setRange(float r){ range = r; }
	float getRange() { return range; }
private:
	float range;
	float distance; // distance traveled
};

