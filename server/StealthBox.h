#pragma once
#pragma once
#include "GameObject.h"
#include "ConfigSettings.h"
class Crusader;
class StealthBox:
	public GameObject
{
public:
	StealthBox();
	~StealthBox();
	StealthBox(Crusader * crus);
	bool updateTime(int time);

	int expandTime;
	int fadeTime;
	int startTime;

	Crusader * crus;
	int crusaderID;
	int stealthed;

	float xMod;
	float yMod;
	float zMod;

	float alpha;


};

