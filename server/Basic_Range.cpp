#include "stdafx.h"
#include "Basic_Range.h"
#include "GameLogic.h"

Basic_Range::Basic_Range()
{
	type = 1;
	cd = 0;
	maxcd = atof(ConfigSettings::config->getValue("ProjectileCD").c_str());;
	ad = atof(ConfigSettings::config->getValue("ProjectileDMG").c_str());;
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

	createSingleProjectile(obj);
}

void Basic_Range::createSingleProjectile(GameObject* obj)
{
	Projectile* p = new Projectile();
	obj->forward.Normalize();
	//p->setID(obj->getID());
	p->setPos(obj->getPos() + obj->forward * 2);
	p->setInitPos(obj->getPos() + obj->forward * 2);
	p->forward = obj->forward;
	p->setRange(c_range);
	p->setDmg(c_ad);
	Packet* packet = new Packet;
	packet->packet_type = ACTION_EVENT;
	packet->id = p->getID();

	float x, y, z; // position, forward direction and speed
	x = p->getPos().x;
	y = p->getPos().y;
	z = p->getPos().z;

	char data[PACKET_DATA_LEN];
	int pointer = 0;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "projectile", 11);
	pointer += 11;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float));
	pointer += sizeof(float);
	data[pointer] = ',';
	pointer++;
	memcpy_s(packet->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
	GameLogic::serverPackets.push_back(packet);
	GameLogic::projectileList.push_back(p);
}

void Basic_Range::createPowerProjectile(GameObject* obj)
{
	Projectile* p = new Projectile();
	obj->forward.Normalize();
	//p->setID(obj->getID());
	p->setPos(obj->getPos() + obj->forward * 2);
	p->setInitPos(obj->getPos() + obj->forward * 2);
	p->forward = obj->forward;
	p->setRange(c_range);
	p->setDmg(c_ad);
	Packet* packet = new Packet;
	packet->packet_type = ACTION_EVENT;
	packet->id = p->getID();

	float x, y, z; // position, forward direction and speed
	x = p->getPos().x;
	y = p->getPos().y;
	z = p->getPos().z;

	char data[PACKET_DATA_LEN];
	int pointer = 0;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "projectile2", 12);
	pointer += 12;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &x, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &y, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &z, sizeof(float));
	pointer += sizeof(float);
	data[pointer] = ',';
	pointer++;
	memcpy_s(packet->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
	GameLogic::serverPackets.push_back(packet);
	GameLogic::projectileList.push_back(p);
}

