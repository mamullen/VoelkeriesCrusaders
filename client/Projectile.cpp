#include "Projectile.h"

Projectile::~Projectile() {
}

void Projectile::init() {
	//fb = MeshLoader("models/MoonModel/MoonModel.fbx");
}

void Projectile::drawObj(){
	//Vector3 min = *getMin();
	//Vector3 max = *getMax();
	//glDisable(GL_CULL_FACE);
	//glColor3f(1.0f, 1.f, 0.f);
	//glutSolidSphere(.4, 30, 30);
	glDisable(GL_LIGHTING);

	glRotatef(rotate, 0.f, 1.f, 0.f);
	glRotatef(180.f, 0.f, 1.f, 0.f);

	g_Model->UpdateAnimation();

	//glEnable(GL_LIGHTING);
	glScalef(2.f, 2.f, 2.f);
	g_Model->Render();
	//glDisable(GL_LIGHTING);

	glEnable(GL_LIGHTING);
}

void Projectile::setRotate(float rot) {
	rotate = rot;
}

void PowerProjectile::drawObj() {
	glDisable(GL_CULL_FACE);
	glColor3f(1.f, 99.0 / 255, 71.0 / 255);
	glutSolidSphere(5, 30, 30);
}