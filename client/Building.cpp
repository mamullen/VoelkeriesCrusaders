#include "Building.h"

void Building::drawObj(){
	Vector3 min = *getMin();
	Vector3 max = *getMax();

	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_QUADS);
	//draw ceiling
	glNormal3f(0, 1, 0);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, max.y, min.z);

	//draw bottom
	glNormal3f(0, 1, 0);
	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, min.y, min.z);

	//right
	glNormal3f(1, 0, 0);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(max.x, min.y, min.z);
	
	//left
	glNormal3f(1, 0, 0);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(min.x, max.y, min.z);

	//front
	glNormal3f(0, 0, 1);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, min.y, min.z);

	//back
	glNormal3f(0, 0, 1);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	
	
}