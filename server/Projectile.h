#pragma once
#include "GameObject.h"
#include "Player.h"

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
	void setInitPos(Vector3& v) { initPos = v; }
private:
	bool collide(GameObject*);
	float hitRadius;
	float range;
	float distance; // distance traveled
	float dmg;      // damage
	float modifier; // dmg modifier
	Vector3 initPos;
};

