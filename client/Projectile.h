////////////
// lauwood
//

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "core.h"
#include "GameObject.h"
#include "MeshLoader.h"

class Projectile : public GameObject {
	Projectile(MeshLoader* model, unsigned int id) : GameObject(model, id) { 
		// implement me
	};

private:
	Vector3 p_Position;
};

#endif