#ifndef WEAPON_H_
#define WEAPON_H_

#include "core.h"
#include "Matrix4x4.h"
#include "GameObject.h"
#include "ConfigSettings.h"

class Weapon : GameObject {
protected:
	MeshLoader* g_Model;

public:
	Weapon(unsigned int id) : GameObject(id) {}
	Weapon(unsigned int id, Vector3 pos) : GameObject(id) {
		SetPosition(pos);
	}
	std::string GetName() { return w_Name; }
	Vector3 GetPosition() { return w_Position; }
	void SetPosition(Vector3 pos) {
		w_Position = pos;
	}

	void GenericDraw(Vector3 scale, Vector3 translate = Vector3()) {
		glPushMatrix();
		glTranslatef(w_Position.x, w_Position.y+translate.y, w_Position.z);
		glRotatef(glfwGetTime() * 30, 0.f, 1.f, 0.f);
		glScalef(scale.x, scale.y, scale.z);
		g_Model->Render();
		glPopMatrix();
	}

protected:
	std::string w_Name;
	Vector3 w_Position;
	float w_Damage;
	float w_Range;
	float w_Ammo;
};

class SunMace : Weapon {
public:
	SunMace(unsigned int id, Vector3 pos) : Weapon(id, pos) {
		g_Model = new MeshLoader((char*)ConfigSettings::config->getValue("SunMace").c_str());
		g_Model->DisableBones();
	}

	void Draw() {
		GenericDraw(Vector3(5.f, 5.f, 5.f));
	}
};

class BatSword : Weapon {
public:
	BatSword(unsigned int id, Vector3 pos) : Weapon(id, pos) {
		g_Model = new MeshLoader((char*)ConfigSettings::config->getValue("BatSword").c_str());
		g_Model->DisableBones();
	}

	void Draw() {
		GenericDraw(Vector3(1.f, 1.f, 1.f));
	}
};

class DefenseShield : Weapon {
public:
	DefenseShield(unsigned int id, Vector3 pos) : Weapon(id, pos) {
		g_Model = new MeshLoader((char*)ConfigSettings::config->getValue("DefenseShield").c_str());
		g_Model->DisableBones();
	}

	void Draw() {
		GenericDraw(Vector3(25.f, 25.f, 25.f), Vector3(0.f, 5.f, 0.f));
	}
};

class LightningBoltAtkSpd : Weapon {
public:
	LightningBoltAtkSpd(unsigned int id, Vector3 pos) : Weapon(id, pos) {
		g_Model = new MeshLoader((char*)ConfigSettings::config->getValue("LightningBoltAtkSpd").c_str());
		g_Model->DisableBones();
	}

	void Draw() {
		GenericDraw(Vector3(1.f, 1.f, 1.f));
	}
};

#endif