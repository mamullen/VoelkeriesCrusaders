#include "stdafx.h"
#include "Crusader.h"


Crusader::Crusader() :Player()
{
	objectType = 4;
	team = 1;
	default_attack_1 = new Basic_Range();
	default_attack_2 = new Basic_Knock();
	setAttack(default_attack_1);
	crusSpeed = default_speed;
}


Crusader::~Crusader()
{
}

Crusader::Crusader(int i) :Player(i)
{
	objectType = 4;
	team = 1;
	default_attack_1 = new Basic_Range();
	default_attack_2 = new Basic_Knock();
	setAttack(default_attack_1);
	crusSpeed = default_speed;
}

void Crusader::updateTime(int time, int delta, std::vector<GameObject*>* obj)
{
	if (time == 0){
		//day
		attack_mode->setRange(atof((ConfigSettings::config->getValue("CrusaderAttackRangeInc").c_str())));
		speed = atof((ConfigSettings::config->getValue("CrusaderMovespeedDay").c_str())) * crusSpeed;
	}
	else if (time == 1){
		//night
		attack_mode->setRange(atof((ConfigSettings::config->getValue("CrusaderAttackRangeDec").c_str())));
		speed = atof((ConfigSettings::config->getValue("CrusaderMovespeedNight").c_str())) * crusSpeed;
	}
}


void Crusader::attack2Start(){

	

	if (attack2started){
		return;
	}
	 

	attack2started = true;
	StealthBox * sb = new StealthBox(this);
	sbox = sb;

	Packet* packet = new Packet;
	packet->packet_type = ACTION_EVENT;
	packet->id = sb->getID();


	float minX = sb->getMin().x;
	float minY = sb->getMin().y;
	float minZ = sb->getMin().z;
	float maxX = sb->getMax().x;
	float maxY = sb->getMax().y;
	float maxZ = sb->getMax().z;
	float alpha = sb->alpha;
	int crusID = sb->crusaderID;
	int stealthed = sb->stealthed;

	char data[PACKET_DATA_LEN];
	int pointer = 0;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "create_stealth", 15);
	pointer += 15;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minX, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minY, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minZ, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxX, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxY, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxZ, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &alpha, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &crusID, sizeof(int));
	pointer += sizeof(int);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &stealthed, sizeof(int));
	pointer += sizeof(int);
	data[pointer] = ',';
	pointer++;
	memcpy_s(packet->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
	GameLogic::serverPackets.push_back(packet);
	GameLogic::stealthBoxList.push_back(sb);
}


void Crusader::attack2EndExtra(){
	// remove the stealthbox
	attack2started = false;
	crusSpeed = default_speed;

	if (sbox == NULL)
		return;

	for (std::vector<StealthBox*>::iterator it = GameLogic::stealthBoxList.begin(); it != GameLogic::stealthBoxList.end(); it++){
		if ((*it)->getID() == sbox->getID()){
			GameLogic::stealthBoxList.erase(it);
			it--;

			Packet* packet = new Packet;
			packet->packet_type = ACTION_EVENT;
			packet->id = sbox->getID();

			float minX = sbox->getMin().x;
			float minY = sbox->getMin().y;
			float minZ = sbox->getMin().z;
			float maxX = sbox->getMax().x;
			float maxY = sbox->getMax().y;
			float maxZ = sbox->getMax().z;
			float alpha = sbox->alpha;
			int crusID = sbox->crusaderID;
			int stealthed = sbox->stealthed;


			char data[PACKET_DATA_LEN];
			int pointer = 0;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "kill_stealth", 13);
			pointer += 13;
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minX, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minY, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &minZ, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxX, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxY, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &maxZ, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &alpha, sizeof(float));
			pointer += sizeof(float);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &crusID, sizeof(int));
			pointer += sizeof(int);
			memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &stealthed, sizeof(int));
			pointer += sizeof(int);
			data[pointer] = ',';
			pointer++;
			memcpy_s(packet->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
			GameLogic::serverPackets.push_back(packet);
		}
	}

}

//void Crusader::attack(GameObject* obj) // does nothing right now
//{
//	if (this->id == obj->getID()){
//		return;
//	}
//	if (this->inRange(obj)){
//		if (obj->objectType != 3){
//			obj->isAttacked(ad);
//		}
//	}
//}