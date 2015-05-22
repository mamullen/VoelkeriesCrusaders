////////////////////////////////////////
// player.cpp
// - author: Wesley Lau
// initial player class for characters
////////////////////////////////////////

#include "player.h"

ObjParser mech;
Player::Player() {
	mech = ObjParser("models/MechAnimation.obj");
	mn = mech.min;
	mx = mech.max;

	collision = false;
	float atkrange = 5;
	playernumber = 0.0;
	health = 100;
}
Player::Player(int pn) {
	mech = ObjParser("models/MechAnimation.obj");
	mn = mech.min;
	mx = mech.max;
	
	collision = false;
	float atkrange = 5;
	playernumber = pn;
	health = 100;
}

void Player::update() {

	//initial player model is just a cone for now
	glPushMatrix();
	
	glRotatef(180, 0, 1, 0);
	glRotatef(rotation, 0, 1, 0);

	mech.Draw();
	
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
bool Player::collide(Floor tile)
{
	
	//Vector4 boundingpositive = Vector4(position.x + radius, position.y + radius, position.z + radius, position.w);
	//Vector4 boundingnegative = Vector4(position.x - radius, position.y - radius, position.z - radius, position.w);
	int x,y,z;
	
	for (int i = 0; i < tile.buildingList.size(); i++)
	{
		
		

		if ((tile.buildingList[i]->min.x <= position.x + mn.x && position.x + mn.x <= tile.buildingList[i]->max.x) || ((tile.buildingList[i]->min.x <= -position.x + mx.x && -position.x + mx.x <= tile.buildingList[i]->max.x)))
			{
				

			if ((tile.buildingList[i]->min.z <= -position.z + mn.z && -position.z + mn.z <= tile.buildingList[i]->max.z) || (tile.buildingList[i]->min.z <= -position.z + mx.z && -position.z + mx.z <= tile.buildingList[i]->max.z))
				{

					
					return true;
				}
			}
		

	}

	return false;
}
bool Player::inrange(Player bob)
{
	if (bob.playernumber == this->playernumber)
	{
		printf("samePlayer\n");
		return false;
	}
	Vector4 facing = position + forward.scale(atkrange);
	Matrix4 inv,trans, rotmx, rotmx2;
	
	rotmx.makeRotateY(45);
	rotmx2.makeRotateY(-45);
	trans.makeTranslate(position.x, position.y, position.z);
	inv = trans;
	inv.makeInverse();
	
	Vector4 r1 = (inv*(rotmx*(trans*facing)));
	Vector4 r2 = (inv*(rotmx*(trans*facing)));

	if ((r1.x <= bob.position.x + bob.mn.x&&bob.position.x + bob.mn.x <= r2.x) || ((r1.x <= bob.position.x + bob.mx.x && bob.position.x + bob.mx.x <= r2.x)))
	{
		if ((r1.z <= bob.position.z + bob.mn.z&&bob.position.z + bob.mn.z <= r2.z) || ((r1.z <= bob.position.z + bob.mx.z && bob.position.z + bob.mx.z <= r2.z)))
		{

			return true;
		}
	}
	return false;
}
void Player::jump() {
	position = position + forward.scale(speed) + up.scale(speed);
}


void Player::MoveForward() {
	position = position + forward.scale(speed);
}

void Player::MoveForward(Floor f) {
	
	if (!collide(f))
	{
		position = position + forward.scale(speed);
	}
}

void Player::MoveForward(float speed, Floor f) {
	if (!collide(f))
	{
		position = position + forward.scale(speed);
	}
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
