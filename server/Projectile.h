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
	void setDmg(float d) { dmg = d; }
	float getRange() { return range; }
private:
	bool collide(GameObject*);
	float hitRadius;
	float range;
	float distance; // distance traveled
	float dmg;      // damage
	float modifier; // dmg modifier
};

