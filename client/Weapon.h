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

	void SetRotation(aiMatrix4x4* rot) {
		/*w_Rotation = Matrix4x4(rot->a1, rot->a2, rot->a3, rot->a4,
							   rot->b1, rot->b2, rot->b3, rot->b4,
							   rot->c1, rot->c2, rot->c3, rot->c4,
							   rot->d1, rot->d2, rot->d3, rot->d4);*/
		w_Rotation = rot;
	}

	void GenericDraw(Vector3 scale, Vector3 translate = Vector3()) {
		glPushMatrix();
		glTranslatef(w_Position.x, w_Position.y+translate.y, w_Position.z);
		if (!w_Equipped) {
			glRotatef(glfwGetTime() * 30, 0.f, 1.f, 0.f);
		}
		glScalef(scale.x, scale.y, scale.z);
		g_Model->IsEquippedWeapon(w_Rotation);
		g_Model->Render();
		glPopMatrix();
	}

	void SetEquipped(bool equip) {
		w_Equipped = equip;
	}

protected:
	std::string w_Name;
	Vector3 w_Position;
	aiMatrix4x4* w_Rotation;
	bool w_Equipped;
};

class SunMace : Weapon {
public:
	SunMace(unsigned int id, Vector3 pos) : Weapon(id, pos) {
		g_Model = new MeshLoader((char*)ConfigSettings::config->getValue("SunMace").c_str());
		g_Model->DisableBones();
	}

	void Draw() {
		GenericDraw(Vector3(1.f, 1.f, 1.f));
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