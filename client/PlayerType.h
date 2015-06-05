#ifndef PLAYERTYPE_H_
#define PLAYERTYPE_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "MeshLoader.h"
#include "objparser.h"
#include "GameObject.h"
#include <iostream>
#include "ConfigSettings.h"
#include "Weapon.h"

class PlayerType: public GameObject {
public:
	PlayerType(unsigned int id, int t) : GameObject(id, t) { setShowHP(true); };
	//Player(unsigned int i, int t) : GameObject(NULL, i){ setShowHP(true); team = t; };

	

	void setHDir(int h) { printf("%d\n", h); hDir = h; }
	void setVDir(int v) { printf("%d\n", v); vDir = v; }
	int getHDir()		{ return hDir; }
	int getVDir()		{ return vDir; }
	void EquipWeapon(Weapon* weapon) {
		equippedWeapon = weapon;
		weapon->SetPosition(Vector3(getPos().x, getPos().y, getPos().z));
	}

	void UpdateMoveAnimation(bool isNight, PlayerType *Player) {
		float speedup;
		if (isNight && Player->getTeam() == 2)
			speedup = 3.f;
		else
			speedup = 2.f;

		if (g_Model != NULL && g_Model->HasAnimations()) {
			if (getVDir() == 1) { // forward
				if (getHDir() == 1) { // forward right
					setAnimation(a_RUNFORWARD, -30, speedup);
				}
				else if (getHDir() == -1) { // forward left
					setAnimation(a_RUNFORWARD, 30, speedup);
				}
				else { // just forward
					setAnimation(a_RUNFORWARD, 0, speedup);
				}
			}
			else if (getVDir() == -1)  { // backward
				if (getHDir() == 1) { // backward right
					setAnimation(a_RUNFORWARD, 30, speedup);
				}
				else if (getHDir() == -1) { // backward left
					setAnimation(a_RUNFORWARD, -30, speedup);
				}
				else { // just backward
					setAnimation(a_WALKBACK, 0, speedup);
				}
			}
			else { // no vdir
				if (getHDir() == 1) { // right
					setAnimation(a_STRAFERIGHT, 0, speedup);
				}
				else if (getHDir() == -1) { // left
					setAnimation(a_STRAFELEFT, 0, speedup);
				}
				else { // idle
					setAnimation(a_IDLE, 0, speedup);
				}
			}
		}
	}

private:
	//vDir: 1 = forward, -1 = backward, 0 = neither
	//hDir: 1 = right, -1 = left, 0 = neither
	int vDir=0, hDir=0;

	Weapon* equippedWeapon;
};

#endif