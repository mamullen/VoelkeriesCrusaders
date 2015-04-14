////////////////////////////////////////
// player.cpp
// - author: Wesley Lau
// initial player class for characters
////////////////////////////////////////

#include "player.h"

void Player::update() {

	//initial player model is just a cone for now
	glPushMatrix();

	glRotatef(180, 0, 1, 0);
	glRotatef(rotation, 0, 1, 0);

	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(-0.6f, 2.f, -0.4f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.6f, 2.f, -0.4f);
	glColor3f(0.f, 1.f, 1.f);
	glVertex3f(0.f, 2.f, 0.6f);

	glEnd();
	glPopMatrix();
}

void Player::jump() {
	position = position + forward.scale(speed) + up.scale(speed);
}

void Player::MoveForward() {
	position = position + forward.scale(speed);
}

void Player::MoveForward(float speed) {
	position = position + forward.scale(speed);
}

void Player::MoveBackward() {
	position = position - forward.scale(speed);
}

void Player::StrafeLeft() {
	position = position - right.scale(speed);
}

void Player::StrafeRight() {
	position = position + right.scale(speed);
}


void Player::rotateLeft() {
	rotation += ROTATE_LEFT;
	rotateMx.MakeRotateY(ROTATE_LEFT*M_PI/180);
	setForward(rotateMx);
}

void Player::rotateRight() {
	rotation += ROTATE_RIGHT;
	rotateMx.MakeRotateY(ROTATE_RIGHT*M_PI/180);
	setForward(rotateMx);
}

void Player::print(std::string str) {
	std::cout << str << " position: " << "x: " << position.x
		<< ", y: " << position.y
		<< ", z: " << position.z
		<< ", forward: " << "x: " << forward.x
		<< ", y: " << forward.y
		<< ", z: " << forward.z
		<< std::endl;
}
