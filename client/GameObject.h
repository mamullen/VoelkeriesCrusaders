#pragma once

#include "core.h"
#include "MeshLoader.h"
#include "matrix34.h"
#include "Vector4.h"
#include "objparser.h"
#include "Shader.h"
#include <iostream>

//should these fields go somewhere else?
/////////////////////////////////////////////////////////////////
static const Vector4 INIT_FORWARD = Vector4(0, 0, 1, 1);
static const Vector4 INIT_RIGHT = Vector4(-1, 0, 0, 1);

// Player's Controls
//movement
const int JUMP = GLFW_KEY_SPACE;
const int FORWARD = GLFW_KEY_W;
const int BACKWARD = GLFW_KEY_S;
const int STRAFELEFT = GLFW_KEY_A;
const int STRAFERIGHT = GLFW_KEY_D;
const int Q = GLFW_KEY_Q;
/////////////////////////////////////////////////////////////////

class GameObject {
public:
	//initialize game object with some model.  If there is no model
	//associated with this GameObject, then just pass in NULL
	GameObject(unsigned int id);
	GameObject(Vector3* mn, Vector3* mx, unsigned int id);

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
	Vector3* getMin()					{ return min; }
	Vector3* getMax()					{ return max; }
	void setName(char* n)				{ name = n; }
	char * getName()					{ return name; }
	void setAnimation(int index, int rotate = 0, float speedup = 2.f);
	bool shrinecollide(Vector3, Vector3);

	void setPos(double x, double y, double z);
	float getRotation()					{ return rotation; }
	void setRotation(float rot);
	void setForward(Matrix34 rotate)	{ rotate.Transform(forward, forward); rotate.Transform(right, right); }
	void resetForward(Matrix34 rotate)	{ rotate.Transform(Vector4(0, 0, 1, 1), forward); rotate.Transform(Vector4(-1, 0, 0, 1), right); }
	void setDashRange(float d)			{ dashRange = d; }
	int getAttacking2Starts()			{ return attack2Starts; }
	int getAttacking2Ends()				{ return attack2Ends; }
	void setAttacking2Starts(int d)		{ attack2Starts = d; }
	void setAttacking2Ends(int d)		{ attack2Ends = d; }
	void setAttacking2(bool t);

	void loadShader(Shader* reg) { 
		p_regShade = reg;
	}

private:
	float animRotate = 0;
	unsigned int id;
	float rotation = 0;
	bool showHP = false;
	float currHP,maxHP;
	Vector3* min, * max;
	char* name = NULL;
	float dashRange = 0;
	bool attacking2 = false;
	int attack2Starts = 0, attack2Ends = 0;

	Vector4 position = Vector4(0, 2, 0, 1);
	Vector4 forward = INIT_FORWARD;
	Vector4 right = INIT_RIGHT;
	Vector4 up = Vector4(0, 1, 0, 1);

	//the model of the object

	Shader* p_regShade;

protected:
	MeshLoader* g_Model;

	Shader* shade;
};

