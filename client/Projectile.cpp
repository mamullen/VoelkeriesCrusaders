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
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); //Enable blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.f, 1.f, 1.f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, proj);
	glBegin(GL_QUADS);
	//glColor3f(1, 1, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-5, 0, -5);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-5, 0, 5);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(5, 0, 5);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(5, 0, -5);
	glEnd();
	glDisable(GL_BLEND);

	
}