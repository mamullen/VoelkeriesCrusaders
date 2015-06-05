#include "Projectile.h"

Projectile::~Projectile() {
}

void Projectile::init() {
	//fb = MeshLoader("models/MoonModel/MoonModel.fbx");
}

void Projectile::drawObj(){
	//Vector3 min = *getMin();
	//Vector3 max = *getMax();
	glDisable(GL_CULL_FACE);
	glColor3f(1.0f, 1.f, 0.f);
	glutSolidSphere(.4, 30, 30);
}

void PowerProjectile::drawObj() {
	glDisable(GL_CULL_FACE);
	glColor3f(1.f, 99.0/255, 71.0/255);
	glutSolidSphere(5, 30, 30);
}