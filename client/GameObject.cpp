#include "GameObject.h"
#include <math.h>

GameObject::GameObject(unsigned int i)
{
	id = i;
}

GameObject::GameObject(Vector3* mn, Vector3* mx, unsigned int i)
{
	min = mn;
	max = mx;
	id = i;
}

void GameObject::update(bool isPlayer, float rot, int t) {

	if (getHealth() <= 0){
		return;
	}

	if (!isVisible()){
		return;
	}

	//initial player model is just a cone for now
	glPushMatrix();
	
	glTranslatef(getPos().x, getPos().y, getPos().z);
	glRotatef(rotation, 0, 1, 0);

	if (name != NULL && !isPlayer){
		p_regShade->unbind();
		glPushMatrix();
		glRotatef(-rotation+180, 0, 1, 0);
		glRotatef(-rot, 0, 1, 0);
		//calculate how much to move name left so to center name
		float paramDist = 40.0f;
		float dist = strlen(name) / 2.0f * (paramDist / 34.0f);
		glTranslatef(-dist, 12, 0);
		glLineWidth(1.4);
		glScalef(paramDist / 3500.0f, paramDist / 3500.0f, paramDist / 3500.0f);
		glColor3f(1, 1, 1);
		if (team == t){
			glColor3f(0, 0, 1);
		}
		for (int i = 0; i<strlen(name); i++){
			//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)name[i]); // generation of characters in our text with 9 by 15 GLU font
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)name[i]);
		}

		glEnd();
		glPopMatrix();
		p_regShade->bind();
	}

	if (showHP && !isPlayer){
		p_regShade->unbind();
		glPushMatrix();
		glRotatef(-rotation, 0, 1, 0);
		glRotatef(-rot, 0, 1, 0);

		glBegin(GL_QUADS);
		glColor3f(0.f, 1.f, 0.f);
		glNormal3f(0, 1, 0);
		glVertex3f(2.f - ((getHealth() / getMaxHealth()))*4.f, 11.f, 0.f); // starting at -2.f
		glVertex3f(2.f, 11.f, 0.f);
		glVertex3f(2.f, 10.f, 0.f);
		glVertex3f(2.f - ((getHealth() / getMaxHealth()))*4.f, 10.f, 0.f);

		
		glColor3f(1.f, 0.f, 0.f);
		glNormal3f(0, 1, 0);
		glVertex3f(-2.f, 11.f, 0.f); // starting at 2.f
		glVertex3f(-2.f + (1-(getHealth() / getMaxHealth()))*4.f, 11.f, 0.f);
		glVertex3f(-2.f + (1-(getHealth() / getMaxHealth()))*4.f, 10.f, 0.f);
		glVertex3f(-2.f, 10.f, 0.f);
		
		glEnd();
		glPopMatrix();
		p_regShade->bind();
	}

	//draw dash range circle if this player is dashing
	if (attacking2 && dashRange > 0){
		p_regShade->unbind();
		glBegin(GL_LINE_LOOP);
		glColor3f(1.f, 0.f, 0.f);
		float radius = dashRange;
		for (int i = 0; i < 360; i++)
		{
			float degInRad = i * (3.14159265f / 180);
			glVertex3f(cos(degInRad)*radius, 2, sin(degInRad)*radius);
		}

		glEnd();
		p_regShade->bind();
	}

	//check  model is not null
	if (g_Model){
		glRotatef(animRotate, 0.f, 1.f, 0.f);
		g_Model->UpdateAnimation();
		g_Model->Render();
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
}

void GameObject::setAnimation(int index, int rotate, float speedup) {
	if (index == a_RUNFORWARD) {
		animRotate = rotate;
	}
	else {
		animRotate = 0;
	}
	g_Model->animRate = speedup;
	g_Model->ChangeAnimation(index);
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

bool GameObject::shrinecollide(Vector3 min, Vector3 max)
{
	if (min.x < position.x && position.x <max.x)
	{
		if (min.z < position.z && position.z < max.z)
		{
			printf("shrine true\n\n");
			return true;
		}
	}
	printf("shrine false\n\n");
	return false;
}

void GameObject::setAttacking2(bool t){
	attacking2 = t;
	dashRange = 0;
}
