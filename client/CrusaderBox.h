#pragma once
#include "GameObject.h"
class CrusaderBox : public GameObject
{
public:
	CrusaderBox(Vector3* min, Vector3* max, float a, unsigned int i) : GameObject(min, max, i){ setMaxHealth(1); setHealth(1); setRotation(0); alpha = a; };
	void drawObj();
	void setAlpha(float a)		{ alpha = a; };
private:
	float alpha;
	//Texture norm;
	//Texture tex;
	//Shader* shade;
};

