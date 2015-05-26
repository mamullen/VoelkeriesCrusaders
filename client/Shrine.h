#ifndef SHRINE_H_
#define SHRINE_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "DaeLoader.h"
#include "objparser.h"
#include "GameObject.h"
#include <iostream>
#include "ConfigSettings.h"

class Shrine : public GameObject {
public:
	Shrine(unsigned int i) : GameObject(new DaeLoader((char*)ConfigSettings::config->getValue("ShrineAnimationFilePath").c_str()), i){ setShowHP(true); };
	//Player(unsigned int i) : GameObject(NULL, i){ setShowHP(true); };
};

#endif
