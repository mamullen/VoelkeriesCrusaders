////////////
// lauwood
//

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "core.h"
#include "GameObject.h"
#include "MeshLoader.h"

#include "matrix34.h"
#include "Vector4.h"
#include "PlayerType.h"
#include <iostream>
#include "ConfigSettings.h"

class Projectile : public GameObject {
public:
	Projectile(unsigned int id) : GameObject(id) { 
		setMaxHealth(1); 
		setHealth(1); 
		//g_Model = new MeshLoader((char*)ConfigSettings::config->getValue("CrusaderFilePath").c_str(),false);
	};
	~Projectile();
	void init();
	void drawObj();

private:
	
};

#endif