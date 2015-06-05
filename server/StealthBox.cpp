#include "stdafx.h"
#include "StealthBox.h"
#include "Crusader.h"


StealthBox::StealthBox()
{
	expandTime = atoi(ConfigSettings::config->getValue("StealthBoxExpand").c_str());
	fadeTime = atoi(ConfigSettings::config->getValue("StealthBoxFade").c_str());
	startTime = 0;
	position = Vector3(0, 0, 0);
	rotation = 0;
	hp = 100;
	objectType = 6;
	crusaderID = -1;
	stealthed = 0;
	alpha = 0;

	xMod = 8;
	yMod = 10;
	zMod = 8;

}

StealthBox::~StealthBox()
{
}

StealthBox::StealthBox(Crusader * c) : StealthBox(){
	crus = c;
	crusaderID = crus->getID();
	setMin(Vector3(crus->getPos().x, crus->getPos().y, crus->getPos().z));
	setMax(Vector3(crus->getPos().x, crus->getPos().y, crus->getPos().z));
}

bool StealthBox::updateTime(int time){
	if (startTime == 0){
		startTime = time; // started expanding
		expandTime = startTime + expandTime; // at what timer time expanding should finish
		fadeTime = expandTime + fadeTime; // at what time timer fading should finish
	}
	else{
		startTime += time;
	}




	float expF = 0;
	//printf("ASDF: %d %d %d\n", startTime, expandTime, fadeTime);
	if (startTime <= expandTime){
		expF = (float)(startTime - time) / (float)expandTime;
		float testMod = xMod * expF;
		setMin(Vector3(crus->getPos().x - (xMod * expF), crus->getPos().y - (yMod * expF), crus->getPos().z - (zMod * expF)));
		setMax(Vector3(crus->getPos().x + (xMod * expF), crus->getPos().y + (yMod+3 * expF), crus->getPos().z + (zMod * expF)));
		alpha = expF;
		if (alpha > 1)
			alpha = 1;
	}
	else if (startTime <= fadeTime){
		stealthed = 1;
		 
		expF = (float)(fadeTime-startTime) / (float)(fadeTime-expandTime);
		//printf("EXPF is %f\n", expF);
		alpha = 0 + expF;
		if (alpha >= .6)
			alpha = 1;
		if (alpha < 0)
			alpha = 0;

		if (expF < 0.1)
			expF = 0.1;

		float mult = 3;
		if (alpha <= .6){
			setMin(Vector3(getMin().x - (xMod / (expF * mult)), getMin().y, getMin().z - (zMod / (expF * mult))));
			setMax(Vector3(getMax().x + (xMod / (expF * mult)), getMax().y + (yMod + 3 / expF), getMax().z + (zMod / (expF * mult))));
		}
		else{
			setMin(Vector3(crus->getPos().x - (xMod * expF), crus->getPos().y - (yMod * expF), crus->getPos().z - (zMod * expF)));
			setMax(Vector3(crus->getPos().x + (xMod * expF), crus->getPos().y + (yMod + 3 * expF), crus->getPos().z + (zMod * expF)));
		}
	}

	if (!(startTime <= expandTime))
		crus->removeHp(atof(ConfigSettings::config->getValue("StealthHealthLoss").c_str()));

	Packet* packet = new Packet;
	packet->packet_type = ACTION_EVENT;
	packet->id = getID();


	float minX = getMin().x;
	float minY = getMin().y;
	float minZ = getMin().z;
	float maxX = getMax().x;
	float maxY = getMax().y;
	float maxZ = getMax().z;
	float alph = alpha;
	int crusID = crusaderID;
	int stealth = stealthed;

	char data[PACKET_DATA_LEN];
	int pointer = 0;
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "stealth", 8);
	pointer += 8;
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
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &alph, sizeof(float));
	pointer += sizeof(float);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &crusID, sizeof(int));
	pointer += sizeof(int);
	memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, &stealth, sizeof(int));
	pointer += sizeof(int);
	data[pointer] = ',';
	pointer++;
	memcpy_s(packet->packet_data, PACKET_DATA_LEN, data, PACKET_DATA_LEN);
	GameLogic::serverPackets.push_back(packet);

}

