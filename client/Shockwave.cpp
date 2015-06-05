#include "Shockwave.h"

#include "CrusaderBox.h"

void Shockwave::drawObj(){
	glEnable(GL_CULL_FACE);
	//shade->bind();
	Vector3 min = *getMin();
	Vector3 max = *getMax();
	glEnable(GL_BLEND); //Enable blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 0,alpha);

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f((min.x+max.x)/2, min.y, (min.z+max.z)/2);

	for (float angle = 1.0f; angle<361.0f; angle += 0.2)
	{
		float x2 = (min.x + max.x) / 2 + sin(angle)*((max.x - min.x) / 2);
		float y2 = (min.z + max.z) / 2 + cos(angle)*((max.x - min.x) / 2);
		glVertex3f(x2,min.y, y2);
	}

	glDisable(GL_BLEND);

	glDisable(GL_CULL_FACE);
}

void Shockwave::decreaseTime(unsigned int t){
	time -= t;
	if (time > 3000){
		time = 0;
	}
	alpha = (float)time / 1500;
}
