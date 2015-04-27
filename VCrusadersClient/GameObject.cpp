#include "GameObject.h"

ObjParser human2;

void GameObject::update() {

	//initial player model is just a cone for now
	glPushMatrix();

	glRotatef(180, 0, 1, 0);
	glRotatef(rotation, 0, 1, 0);
	
	human2.ParserDraw();

	/*glBegin(GL_TRIANGLES);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(-0.6f, 2.f, -0.4f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.6f, 2.f, -0.4f);
	glColor3f(0.f, 1.f, 1.f);
	glVertex3f(0.f, 2.f, 0.6f);*/
	
	glEnd();
	glPopMatrix();
}

void GameObject::jump() {
	position = position + forward.scale(speed) + up.scale(speed);
}

void GameObject::moveToCoord(double x, double y, double z){
	position = Vector4(x, y, z, 1);
}

void GameObject::rotateLeft() {
	rotation += ROTATE_LEFT;
	rotateMx.MakeRotateY(ROTATE_LEFT*M_PI / 180);
	setForward(rotateMx);
}

void GameObject::rotateRight() {
	rotation += ROTATE_RIGHT;
	rotateMx.MakeRotateY(ROTATE_RIGHT*M_PI / 180);
	setForward(rotateMx);
}

void GameObject::print(std::string str) {
	std::cout << str << " position: " << "x: " << position.x
		<< ", y: " << position.y
		<< ", z: " << position.z
		<< ", forward: " << "x: " << forward.x
		<< ", y: " << forward.y
		<< ", z: " << forward.z
		<< std::endl;
}

