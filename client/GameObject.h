#pragma once

#include "core.h"
#include "DaeLoader.h"
#include "matrix34.h"
#include "Vector4.h"
#include "objparser.h"
#include <iostream>

//should these fields go somewhere else?
/////////////////////////////////////////////////////////////////
static const Vector4 INIT_FORWARD = Vector4(0, 0, 1, 1);
static const Vector4 INIT_RIGHT = Vector4(-1, 0, 0, 1);

// Player's Controls
const int FORWARD = GLFW_KEY_W;
const int BACKWARD = GLFW_KEY_S;
const int STRAFELEFT = GLFW_KEY_A;
const int STRAFERIGHT = GLFW_KEY_D;
/////////////////////////////////////////////////////////////////

class GameObject {
public:
	//initialize game object with some model.  If there is no model
	//associated with this GameObject, then just pass in NULL
	GameObject(DaeLoader* m, unsigned int i);

	void rotateLeft();
	void rotateRight();
	void update(bool isPlayer,float rot);
	virtual void drawObj(); //will draw a triangle if not overridden
	void print(std::string);

	unsigned int getID()				{ return id; }
	float getHealth()						{ return currHP; }
	void setHealth(float h);
	float getMaxHealth()					{ return maxHP; }
	void setMaxHealth(float h);
	bool getShowHP()					{ return showHP; }
	void setShowHP(bool t)				{ showHP = t; }
	Vector4 getPos()					{ return position; }
	void setPos(double x, double y, double z);
	float getRotation()					{ return rotation; }
	void setRotation(float rot);
	void setForward(Matrix34 rotate)	{ rotate.Transform(forward, forward); rotate.Transform(right, right); }
	void resetForward(Matrix34 rotate)	{ rotate.Transform(Vector4(0, 0, 1, 1), forward); rotate.Transform(Vector4(-1, 0, 0, 1), right); }

private:
	unsigned int id;
	float rotation = 0;
	bool showHP = false;
	float currHP,maxHP;

	Vector4 position = Vector4(0, 2, 0, 1);
	Vector4 forward = INIT_FORWARD;
	Vector4 right = INIT_RIGHT;
	Vector4 up = Vector4(0, 1, 0, 1);

	//the model of the object
	DaeLoader* model;
};

