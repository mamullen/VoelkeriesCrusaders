#ifndef WEAPON_H_
#define WEAPON_H_

#include "core.h"
#include "Matrix4x4.h"
#include "GameObject.h"
#include "ConfigSettings.h"

//TODO: plz add to server as gameobject
class Weapon : GameObject {
protected:
	MeshLoader* g_Model;

public:
	Weapon(unsigned int id) : GameObject(id) {}
	virtual Vector3 GetPosition() { return w_Position; }

protected:
	Vector3 w_Position;
	float w_Damage;
	float w_Range;
	float w_Ammo;
	float w_MaxAmmo;
};

class SunMace : Weapon {
public:
	SunMace(unsigned int id) : Weapon(id) {
		g_Model = new MeshLoader((char*)ConfigSettings::config->getValue("SunMace").c_str());
		g_Model->DisableBones();
	}

	void Draw() {
		glPushMatrix();
		glTranslatef(-75.f, -1.f, -60.f);
		glScalef(5.f, 5.f, 5.f);
		g_Model->Render();
		glPopMatrix();
	}
};

#endif