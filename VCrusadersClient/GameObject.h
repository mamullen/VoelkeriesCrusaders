#pragma once

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "objparser.h"
#include <iostream>

class GameObject {
public:
	void moveToCoord(double x, double y, double z);

	void rotateLeft();
	void rotateRight();
	void jump();
	void update();
	void print(std::string);

	Vector4 getPos()					{ return position; }
	float getRotation()					{ return rotation; }
	void setRotation(float rot)			{ rotation = rot; rotateMx.MakeRotateY(rot*M_PI / 180); resetForward(rotateMx); }
	void setForward(Matrix34 rotate)	{ rotate.Transform(forward, forward); rotate.Transform(right, right); }
	void resetForward(Matrix34 rotate)	{ rotate.Transform(Vector4(0, 0, 1, 1), forward); rotate.Transform(Vector4(-1, 0, 0, 1), right); }

private:
	float speed = 0.5;
	float rotation = 0;
	Matrix34 rotateMx;

	Vector4 position = Vector4(0, 2, 0, 1);
	Vector4 forward = Vector4(0, 0, 1, 1);
	Vector4 right = Vector4(-1, 0, 0, 1);
	Vector4 up = Vector4(0, 1, 0, 1);
};

