#include "GameObject.h"
#include <math.h>

GameObject::GameObject(MeshLoader* m, unsigned int i)
{
	model = m;
	id = i;
}

GameObject::GameObject(MeshLoader* m, Vector3* mn, Vector3* mx, unsigned int i)
{
	model = m;
	min = mn;
	max = mx;
	id = i;
}

void GameObject::update(bool isPlayer, float rot) {

	if (getHealth() <= 0){
		return;
	}

	//initial player model is just a cone for now
	glPushMatrix();
	
	glTranslatef(getPos().x, getPos().y, getPos().z);
	glRotatef(rotation, 0, 1, 0);

	if (name != NULL && !isPlayer){
		glPushMatrix();
		glRotatef(-rotation+180, 0, 1, 0);
		glRotatef(-rot, 0, 1, 0);
		//calculate how much to move name left so to center name
		float paramDist = 40.0f;
		float dist = strlen(name) / 2.0f * (paramDist / 34.0f);
		glTranslatef(-dist, 5, 0);
		glLineWidth(3);
		glScalef(paramDist / 3500.0f, paramDist / 3500.0f, paramDist / 3500.0f);
		glColor3f(1, 1, 1);
		for (int i = 0; i<strlen(name); i++){
			//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)name[i]); // generation of characters in our text with 9 by 15 GLU font
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)name[i]);
		}

		glEnd();
		glPopMatrix();
	}

	if (showHP){
		glPushMatrix();
		if (!isPlayer){
			glRotatef(-rotation, 0, 1, 0);
			glRotatef(-rot, 0, 1, 0);
		}

		glBegin(GL_QUADS);
		glColor3f(0.f, 1.f, 0.f);
		glNormal3f(0, 1, 0);
		glVertex3f(2.f - ((getHealth() / getMaxHealth()))*4.f, 4.f, 0.f); // starting at -2.f
		glVertex3f(2.f, 4.f, 0.f);
		glVertex3f(2.f, 3.f, 0.f);
		glVertex3f(2.f - ((getHealth() / getMaxHealth()))*4.f, 3.f, 0.f);

		
		glColor3f(1.f, 0.f, 0.f);
		glNormal3f(0, 1, 0);
		glVertex3f(-2.f, 4.f, 0.f); // starting at 2.f
		glVertex3f(-2.f + (1-(getHealth() / getMaxHealth()))*4.f, 4.f, 0.f);
		glVertex3f(-2.f + (1-(getHealth() / getMaxHealth()))*4.f, 3.f, 0.f);
		glVertex3f(-2.f, 3.f, 0.f);
		
		glEnd();
		glPopMatrix();
	}
	
	//check  model is not null
	if (model){
		model->UpdateAnimation();
		model->Render();
	}
	else {
		//no model, so use draw method to draw object
		drawObj();
	}
	
	glEnd();
	glPopMatrix();
}

void GameObject::drawObj(){
	//draws a simple triangle
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 1.f, 0.f);
	glVertex3f(-0.6f, 2.f, -0.4f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.6f, 2.f, -0.4f);
	glColor3f(0.f, 1.f, 1.f);
	glVertex3f(0.f, 2.f, 0.6f);
}

void GameObject::setPos(double x, double y, double z){
	position = Vector4(x, y, z, 1);
}

void GameObject::setRotation(float r){
	rotation = r;
}

void GameObject::setMaxHealth(float h){
	maxHP = h;
}

void GameObject::setHealth(float h){
	currHP = h;
	if (currHP > maxHP){
		currHP = maxHP;
	}
	else if (currHP < 0){
		currHP = 0;
	}
	printf("NEW HEALTH VAL: %f\n", currHP);
}

void GameObject::setAnimation(int index) {
	model->ChangeAnimation(index);
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

