#pragma once
#include "GameObject.h"
class Building :
	public GameObject
{
public:
	Building();
	virtual ~Building();
	virtual void isAttacked(float);
	virtual void update(Packet*, std::vector<GameObject*>*);
	/*
	void setMin(float,float,float);
	void setMax(float,float,float);
	void setMin(Vector3&);
	void setMax(Vector3&);
	Vector3 getMin();
	Vector3 getMax();
	*/

private:
	//Vector3 min, max;
};

