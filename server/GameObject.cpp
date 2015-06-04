#include "stdafx.h"
#include "GameObject.h"

unsigned int GameObject::totalId;
std::list<std::pair<int, std::string*>> GameObject::changes;

GameObject::GameObject()
{
	id = totalId;
	totalId++;
	isPlayer = false;
	objectType = -1;
	ground = 1;
	landed = true;
	speed = 0.2;
	/*
	position = Vector3(0, 0, 0);
	pPosition = Vector3(position.x, position.y, position.z);
	forward = Vector3(0, 0, 1);
	pForward = Vector3(forward.x, forward.y, forward.z);
	right = Vector3(-1, 0, 0);
	up = Vector3(0, 1, 0);
	*/
}

GameObject::~GameObject()
{
}


void GameObject::setPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void GameObject::setPos(Vector3& v)
{
	position = v;
}

void GameObject::moveForward()
{
	position = position + forward*speed;
}

void GameObject::gravity()
{

	if (position.y > ground)
	{
		position = position - 0.3*up;

	}
}

void GameObject::jump()
{
	if (landed)
	{
		position = position + up*2;
		
	}
	if (position.y >ground + 4)
	{
		landed = false;
	}
	//if (!landed)
	//{
	//gravity();
	//}
	if (position.y <= ground)
	{
		landed = true;
	}
}

void GameObject::moveBackward()
{
	position = position - forward*speed;
}



void GameObject::strafeLeft()
{
	position = position - right*speed;
}

void GameObject::strafeRight()
{
	position = position + right*speed;
}

void GameObject::setRotation(float f){
	rotation = f;
	rotateMx.MakeRotateY(rotation*PI / 180);
	forward = rotateMx.c;
	right = -1 * rotateMx.a;
}

void GameObject::rotLeft()
{
	rotation += ROTATE_LEFT;
	rotateMx.MakeRotateY(rotation*PI / 180);
	forward = rotateMx.c;
	right = -1 * rotateMx.a;
}

void GameObject::rotRight()
{
	rotation += ROTATE_RIGHT;
	rotateMx.MakeRotateY(rotation*PI / 180);
	forward = rotateMx.c;
	right = -1 * rotateMx.a;
}

Vector3 GameObject::getPos()
{
	return position;
}

float GameObject::getRot()
{
	return rotation;
}

int GameObject::getID()
{
	return id;
}

void GameObject::setID(int theID){
	id = theID;
}

void GameObject::clearChanges()
{
	//changeList.clear();
	for (int i = 0; i < attr_num; i++){
		isChanged[i] = false;
		change_counter[i] = 0;
	}
}

float GameObject::getHP()
{
	return hp;
}


void GameObject::setMin(float x, float y, float z)
{
	min.Set(x, y, z);
}

void GameObject::setMax(float x, float y, float z)
{
	max.Set(x, y, z);
}

void GameObject::setMin(Vector3& v)
{
	min = v;
}

void GameObject::setMax(Vector3& v)
{
	max = v;
}

Vector3 GameObject::getMin(){
	return min;
}

Vector3 GameObject::getMax(){
	return max;
}

/*
std::map<std::string*, bool> GameObject::getChanges()
{
	//check if any properties of this object have changed from previous check
	if (position.x != pPosition.x || position.y != pPosition.y || position.z != pPosition.z)
	{
		pPosition.x = position.x;
		pPosition.y = position.y;
		pPosition.z = position.z;
		std::string* change = new std::string("pos");
		addChange(change);
	}
	if (forward.x != pForward.x || forward.y != pForward.y || forward.z != pForward.z)
	{
		pForward.x = forward.x;
		pForward.y = forward.y;
		pForward.z = forward.z;
		std::string* change = new std::string("dir");
		addChange(change);
	}

	return changeList;
}

void GameObject::addChange(std::string* change)
{
	changeList.insert(std::pair<std::string*,bool>(change,true));
}
*/
