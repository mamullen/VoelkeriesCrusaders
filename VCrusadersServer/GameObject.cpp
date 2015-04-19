#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(int i)
{
	id = i;
	position = Vector3(0, 0, 0);
	pPosition = Vector3(position.x, position.y, position.z);
	forward = Vector3(0, 0, 1);
	pForward = Vector3(forward.x, forward.y, forward.z);
	right = Vector3(-1, 0, 0);
	up = Vector3(0, 1, 0);
}

GameObject::~GameObject()
{
}

void GameObject::moveForward()
{
	position = position + forward*speed;
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

Vector3 GameObject::getPos()
{
	return position;
}

int GameObject::getID()
{
	return id;
}

std::map<char*, bool> GameObject::getChanges()
{
	//check if any properties of this object have changed from previous check
	if (position.x != pPosition.x || position.y != pPosition.y || position.z != pPosition.z)
	{
		pPosition.x = position.x;
		pPosition.y = position.y;
		pPosition.z = position.z;
		addChange("pos");
	}
	if (forward.x != pForward.x || forward.y != pForward.y || forward.z != pForward.z)
	{
		pForward.x = forward.x;
		pForward.y = forward.y;
		pForward.z = forward.z;
		addChange("dir");
	}

	return changeList;
}

void GameObject::addChange(char* change)
{
	changeList.insert(std::pair<char*,bool>(change,true));
}

void GameObject::clearChanges()
{
	changeList.clear();
}