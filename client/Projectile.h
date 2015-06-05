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
#include "ParticleSystem.h"
#include "Emitter.h"

class Projectile : public GameObject {
public:
	Projectile(unsigned int id) : GameObject(id) {
		setMaxHealth(1); 
		setHealth(1); 
	};
	~Projectile();
	void init();
	void drawObj();

private:
	
};

class PowerProjectile : public Projectile {
public:
	PowerProjectile(unsigned int id) : Projectile(id) {
		setMaxHealth(1);
		setHealth(1);
	}
	~PowerProjectile();
	void drawObj();
};

#endif