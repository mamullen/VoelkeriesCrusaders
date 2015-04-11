////////////////////////////////////////
// core.cpp
////////////////////////////////////////

#include "core.h"

////////////////////////////////////////////////////////////////////////////////

void drawAxis(float size) {
	glBegin(GL_LINES);
		glColor4f(1.0f,0.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(size,0.0f,0.0f);

		glColor4f(0.0f,1.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,size,0.0f);

		glColor4f(0.0f,0.0f,1.0f,1.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,size);

		glColor4f(1.0f,1.0f,1.0f,1.0f);
	glEnd();
}

////////////////////////////////////////////////////////////////////////////////

void drawWireBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax) {
	glPushMatrix();
		glTranslatef(0.5f*(xmin+xmax),0.5f*(ymin+ymax),0.5f*(zmin+zmax));
		glScalef(xmax-xmin,ymax-ymin,zmax-zmin);
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glutWireCube(1.0f);
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////
