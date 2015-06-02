#include "Projectile.h"

void Projectile::drawObj(){
	Vector3 min = *getMin();
	Vector3 max = *getMax();
	//glDisable(GL_CULL_FACE);
	glColor3f(1.0f, 1.f, 0.f);

	glutSolidSphere(.4, 30, 30);

}