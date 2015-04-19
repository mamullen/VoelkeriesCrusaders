#include "stdafx.h"
#include "GameObject.h"

GameObject::~GameObject()
{
}

void GameObject::moveForward()
{
	yPos += speed;
}

void GameObject::moveBackward()
{
	yPos -= speed;
}

void GameObject::strafeLeft()
{
	xPos -= speed;
}

void GameObject::strafeRight()
{
	xPos += speed;
}

double GameObject::getX()
{
	return xPos;
}

double GameObject::getY()
{
	return yPos;
}

double GameObject::getZ()
{
	return zPos;
}

int GameObject::getID()
{
	return id;
}