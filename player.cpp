////////////////////////////////////////
// player.cpp
// - author: Wesley Lau
// initial player class for characters
////////////////////////////////////////

#include "player.h"

void Player::update() {

	//initial player model is just a cone for now
	glPushMatrix();
	glColor3f(0, 1, 0);
	glRotatef(180, 0, 1, 0);
	glRotatef(rotation, 0, 1, 0);
	glutSolidCone(0.5, 1, 20, 20);
	glPopMatrix();
}

void Player::jump() {
	position = position + forward.scale(speed) + up.scale(speed);
}

void Player::moveForward() {
	position = position + forward.scale(speed);
}

void Player::moveForward(float speed) {
	position = position + forward.scale(speed);
}

void Player::moveBackward() {
	position = position - forward.scale(speed);
}

void Player::moveRight() {
	position = position + right.scale(speed);
}

void Player::moveLeft() {
	position = position - right.scale(speed);
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
