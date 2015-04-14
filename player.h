#ifndef PLAYER_H_
#define PLAYER_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include <iostream>

static const Vector4 INIT_FORWARD = Vector4(0, 0, 1, 1);
static const Vector4 INIT_RIGHT = Vector4(-1, 0, 0, 1);

class Player {
private:
	float speed = 0.5;
	float rotation = 0;
	Matrix34 rotateMx;

	Vector4 position = Vector4(0, 2, 0, 1);
	Vector4 forward = INIT_FORWARD;
	Vector4 right = INIT_RIGHT;
	Vector4 up = Vector4(0, 1, 0, 1); 

public:
	void moveForward();
	void moveForward(float speed);
	void moveBackward();
	void moveLeft();
	void moveRight();
	void rotateLeft();
	void rotateRight();
	void jump();
	//void roll(); 
	void update();
	void print(std::string);

	Vector4 getPos()					{ return position; }
	float getRotation()					{ return rotation; }
	void setRotation(float rot)			{ rotation = rot; rotateMx.MakeRotateY(rot*M_PI/180); resetForward(rotateMx); }
	void setForward(Matrix34 rotate)	{ rotate.Transform(forward, forward); rotate.Transform(right, right); }
	void resetForward(Matrix34 rotate)	{ rotate.Transform(INIT_FORWARD, forward); rotate.Transform(INIT_RIGHT, right); }

};

#endif