////////////
// lauwood
//

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "core.h"
#include "GameObject.h"
#include "MeshLoader.h"

class Projectile : public GameObject {
public:
	Projectile(unsigned int id) : GameObject(id) { setMaxHealth(1); setHealth(1); };
	void drawObj();
};

#endif