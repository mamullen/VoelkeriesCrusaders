#pragma once
#include "GameObject.h"
#include "Player.h"

class Projectile :
	public GameObject
{
public:
	Projectile();
	virtual ~Projectile();

	virtual bool updateTime(int time, std::vector<GameObject*>*);
	void setRange(float r){ range = r; }
	void setDmg(float d) { dmg = d; }
	float getRange() { return range; }
	void setInitPos(Vector3& v) { initPos = v; }
	void setRecoil(bool r) { isRecoil = r; }
	void setStun(bool r) { isStun = r; }
	void sendDeathPacket();
	void sendPosPacket();
	void setHitRadius(float r) { hitRadius = r; }

	float recoilAmount;

protected:
	bool collide(GameObject*);
	bool collideWall(std::vector<GameObject*>* obj,int time);
	float hitRadius;
	float range;
	float distance; // distance traveled
	float dmg;      // damage
	float modifier; // dmg modifier
	bool isRecoil;
	bool isStun;
	Vector3 initPos;
};

