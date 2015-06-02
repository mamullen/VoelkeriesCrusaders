#pragma once

#include <stdio.h>
#include "GameObject.h"
#include "Shader.h"

class Building : public GameObject{

public:
	Building(Vector3* min, Vector3* max, float rot, unsigned int i) : GameObject(min, max, i){ setMaxHealth(1); setHealth(1); setRotation(rot); };
	void drawObj();
	GLuint norm;
	GLuint tex;
	Shader* shade;
};
