////////////////////////////////////////
// Light.h
////////////////////////////////////////

#ifndef LIGHT_H
#define LIGHT_H

#include "core.h"
#include "ClientGame.h"

class Light {
public:
	Light();
	Light(float R, float G, float B) : r(R), g(G), b(B) {}
	void Set();
	void Init();
	void Draw(ClientGame* client, int currGameTime);

	float MIN_LIGHT;
	float START_LIGHT;

private:
	float r, g, b;
	float r1, g1, b1;
};

#endif