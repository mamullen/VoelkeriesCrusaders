#ifndef VECTOR2_H
#define VECTOR2_H

#include "core.h"

class Vector2 {
public:
	Vector2() : x(0.0f), y(0.0f)				{}
	Vector2(float f) : x(f), y(f)				{}
	Vector2(float x0, float y0) : x(x0), y(y0)	{}
	void Set(float x0, float y0)				{ x = x0; y = y0; }

private:
	float x, y;
};

#endif