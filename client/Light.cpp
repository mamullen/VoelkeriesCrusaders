#include "Light.h"

void Light::Init() {
	glEnable(GL_LIGHTING);

	r = g = b = 0.6;

	Set();

	GLfloat light_diffuse[] = { r, g, b };
	GLfloat light_position[] = { 200.0, 180.0, 200.0 };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void Light::Set() {
	glEnable(GL_LIGHTING);
	GLfloat light_diffuse[] = { r, g, b };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

void Light::Draw(ClientGame* client, int currGameTime) {
	float p1t = client->getPhase1Time();
	float p2t = client->getPhase2Time();
	float p3t = client->getPhase3Time();

	//std::cout << "phase 1 is: " << p1t << std::endl;

	if (currGameTime < p1t)
	{
		float_t dif = ((float_t)currGameTime / p1t) * 0.7;
		r = g = b = 1 - dif;
	}
	else if (currGameTime < p2t)
	{
		//float_t dif = ((float_t)(currGlight_meTime - p1t) / (p2t - p1t)) / 2;
		r = g = b = 0.3;
	}
	else if (currGameTime < p3t)
	{
		float_t dif = ((float_t)(currGameTime - p2t) / (p3t - p2t)) * 0.7;
		r = g = b = 0.3 + dif;

		//std::cout << "color: " << r << std::endl;
	}
	Set();
}