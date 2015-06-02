////////////////////////////////////////
// Light.h
////////////////////////////////////////

#ifndef LIGHT_H
#define LIGHT_H

#include "core.h"
#include "ClientGame.h"

class Light {
public:
	Light() : r(1), g(1), b(1) {}
	Light(float R, float G, float B) : r(R), g(G), b(B) {}
	void Set();
	void Init();
	void Draw(ClientGame* client, int currGameTime);

private:
	float r, g, b;
};

#endif