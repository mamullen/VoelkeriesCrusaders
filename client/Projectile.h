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
		//g_Model = new MeshLoader("models/Fireslash.fbx", true);
		g_Model = new MeshLoader((char*)ConfigSettings::config->getValue("Fireslash").c_str(), true);
		g_Model->DisableBones();
		rotate = 0;
		setProj(true);
	};
	~Projectile();
	void init();
	void drawObj();
	void setRotate(float rot);

private:
	float rotate;
};

class PowerProjectile : public Projectile {
public:
	PowerProjectile(unsigned int id) : Projectile(id) {
		setMaxHealth(1);
		setHealth(1);
		string filenm = "./ppm/bat.png";
		proj = SOIL_load_OGL_texture(filenm.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	}
	~PowerProjectile();
	void drawObj();
	GLuint proj;
};

#endif