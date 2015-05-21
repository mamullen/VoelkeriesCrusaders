#include "stdafx.h"
#include "Building.h"


Building::Building()
{
	// attributes
	position = Vector3(0, 1.3, 0);
	rotation = 0;
	hp = 100;

	attr_num = 3;
	isChanged = new bool[attr_num];
	change_counter = new int[attr_num];
	for (int i = 0; i < attr_num; i++){
		isChanged[i] = false;
		change_counter[i] = 0;
	}
}


Building::~Building()
{
	delete[] isChanged;
	delete[] change_counter;
}

void Building::setMin(float x, float y, float z)
{
	min.Set(x, y, z);
}

void Building::setMax(float x, float y, float z)
{
	max.Set(x, y, z);
}

void Building::setMin(Vector3& v)
{
	min = v;
}

void Building::setMax(Vector3& v)
{
	max = v;
}

void Building::isAttacked(float ad){

}

void Building::update(Packet*, std::vector<GameObject*>*)
{

}