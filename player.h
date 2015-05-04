#ifndef PLAYER_H_
#define PLAYER_H_

#include "core.h"
#include "DaeLoader.h"
#include "matrix34.h"

#include "Vector4.h"
#include "objparser.h"
#include <iostream>

static const Vector4 INIT_FORWARD = Vector4(0, 0, 1, 1);
static const Vector4 INIT_RIGHT = Vector4(-1, 0, 0, 1);

// Player's Controls
const int FORWARD = GLFW_KEY_W;
const int BACKWARD = GLFW_KEY_S;
const int STRAFELEFT = GLFW_KEY_Q;
const int STRAFERIGHT = GLFW_KEY_E;
const int ROTATELEFT = GLFW_KEY_A;
const int ROTATERIGHT = GLFW_KEY_D;

class Player {
private:
	float speed = 2;
	float rotation = 0;
	Matrix34 rotateMx;

	Vector4 position = Vector4(0, 2, 0, 1);
	Vector4 forward = INIT_FORWARD;
	Vector4 right = INIT_RIGHT;
	Vector4 up = Vector4(0, 1, 0, 1); 

public:
	Player();
	void MoveForward();
	void MoveForward(float speed);
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();

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