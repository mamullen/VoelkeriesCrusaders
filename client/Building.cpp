#include "Building.h"

void Building::drawObj(){
	printf("DRAWING\n");
	Vector3 min = getMin();
	Vector3 max = getMax();

	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	//should draw ceiling
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, max.y, min.z);
}