#include "Light.h"

Light::Light(){
	MIN_LIGHT = atof(ConfigSettings::config->getValue("MinLight").c_str());
	START_LIGHT = atof(ConfigSettings::config->getValue("StartLight").c_str());
	r = g = b = START_LIGHT;
	r1 = 1.0;
	g1 = 1.0;
	b1 = 0.4;
	Init();
}

void Light::Init() {
	glEnable(GL_LIGHTING);



	r = g = b = START_LIGHT;

	Set();

	GLfloat light_diffuse[] = { r, g, b };
	GLfloat light_position[] = { 200.0, 180.0, 200.0 };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat light_diffuse1[] = { r1, g1, b1 };
	GLfloat light_position1[] = { 0.0, 100.0, 0.0 };
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
}

void Light::Set() {
	glEnable(GL_LIGHTING);
	GLfloat light_diffuse[] = { r, g, b };
	GLfloat light_diffuse1[] = { r1, g1, b1 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
}

void Light::Draw(ClientGame* client, int currGameTime) {
	float p1t = client->getPhase1Time();
	float p2t = client->getPhase2Time();
	float p3t = client->getPhase3Time();

	//std::cout << "phase 1 is: " << p1t << std::endl;

	if (currGameTime < p1t)
	{
		float_t dif = ((float_t)currGameTime / p1t) * (1 - MIN_LIGHT);
		r = g = b = 1 - dif;
		g1 = b1 = 1 - dif;
		r1 = (1 - dif) * 0.4;
	}
	else if (currGameTime < p2t)
	{
		//float_t dif = ((float_t)(currGlight_meTime - p1t) / (p2t - p1t)) / 2;
		r = g = b = MIN_LIGHT;
		g1 = b1 = MIN_LIGHT;
		r1 = 0.4 * MIN_LIGHT;
	}
	else if (currGameTime < p3t)
	{
		float_t dif = ((float_t)(currGameTime - p2t) / (p3t - p2t)) * (1 - MIN_LIGHT);
		r = g = b = MIN_LIGHT + dif;
		g1 = b1 = MIN_LIGHT + dif;
		r1 = 0.4 * (MIN_LIGHT + dif);

		//std::cout << "color: " << r << std::endl;
	}
	Set();
}