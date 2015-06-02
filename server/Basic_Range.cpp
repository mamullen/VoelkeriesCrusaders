#include "stdafx.h"
#include "Basic_Range.h"
#include "GameLogic.h"

Basic_Range::Basic_Range()
{
	type = 1;
	cd = 0;
	maxcd = 2;
	ad = 10;
	range = 200;
	c_ad = ad;
	c_range = range;
}


Basic_Range::~Basic_Range()
{
}

void Basic_Range::attack(GameObject* obj)
{
	if (getCD() > 0){
		return;
	}
	maxCD();

	Projectile* p = new Projectile();
	obj->forward.Normalize();
	p->setID(obj->getID());
	p->setPos(obj->getPos());
	p->setInitPos(obj->getPos());
	p->forward = obj->forward;
	p->setRange(c_range);
	p->setDmg(c_ad);
	Packet* packet = new Packet;
	packet->packet_type = ACTION_EVENT;
	packet->id = 0;

	float x, y, z; // position, forward direction and speed
	x = p->getPos().x;
	y = p->getPos().y;
	z = p->getPos().z;

	char data[PACKET_DATA_LEN];
	int pointer = 0;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "projectile",11);
	pointer += 11;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float));
	pointer += sizeof(float);
	data[pointer] = ',';
	pointer++;
	GameLogic::serverPackets.push_back(packet);
	GameLogic::projectileList.push_back(p);
}
