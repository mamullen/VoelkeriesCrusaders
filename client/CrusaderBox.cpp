#include "CrusaderBox.h"

void CrusaderBox::drawObj(){
	glEnable(GL_CULL_FACE);
	//shade->bind();
	Vector3 min = *getMin();
	Vector3 max = *getMax();

	/*int texture_location = glGetUniformLocation(shade->id(), "color_texture");
	glUniform1i(texture_location, 0);
	tex.Bind(GL_TEXTURE0);

	int normal_location = glGetUniformLocation(shade->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	norm.Bind(GL_TEXTURE1);*/
	glEnable(GL_BLEND); //Enable blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(.5,.5,.5,alpha);
	//draw ceiling
	glNormal3f(0, 1, 0);
	//glTexCoord2f(0.0f, 0.0f);
	glVertex3f(min.x, max.y, min.z);
	//glTexCoord2f(0.0f, 1.0f);
	glVertex3f(min.x, max.y, max.z);
	//glTexCoord2f(1.0f, 1.0f);
	glVertex3f(max.x, max.y, max.z);
	//glTexCoord2f(1.0f, 0.0f);
	glVertex3f(max.x, max.y, min.z);

	//draw bottom
	//glNormal3f(0, 1, 0);
	//glVertex3f(max.x, min.y, min.z);
	//glVertex3f(max.x, min.y, max.z);
	//glVertex3f(min.x, min.y, max.z);
	//glVertex3f(min.x, min.y, min.z);

	//right
	glNormal3f(1, 0, 0);
	//glTexCoord2f(0.0f, 0.0f);
	glVertex3f(max.x, max.y, min.z);
	//glTexCoord2f(1.0f, 0.0f);
	glVertex3f(max.x, max.y, max.z);
	//glTexCoord2f(1.0f, 1.0f);
	glVertex3f(max.x, min.y, max.z);
	//glTexCoord2f(0.0f, 1.0f);
	glVertex3f(max.x, min.y, min.z);

	//left
	glNormal3f(1, 0, 0);
	//glTexCoord2f(0.0f, 0.0f);
	glVertex3f(min.x, min.y, min.z);
	//glTexCoord2f(1.0f, 0.0f);
	glVertex3f(min.x, min.y, max.z);
	//glTexCoord2f(1.0f, 1.0f);
	glVertex3f(min.x, max.y, max.z);
	//glTexCoord2f(0.0f, 1.0f);
	glVertex3f(min.x, max.y, min.z);

	//front

	glNormal3f(0, 0, 1);
	//glTexCoord2f(0.0f, 0.0f);
	glVertex3f(max.x, max.y, min.z);
	//glTexCoord2f(0.0f, 1.0f);
	glVertex3f(max.x, min.y, min.z);
	//glTexCoord2f(1.0f, 1.0f);
	glVertex3f(min.x, min.y, min.z);
	//glTexCoord2f(1.0f, 0.0f);
	glVertex3f(min.x, max.y, min.z);


	//back
	glNormal3f(0, 0, 1);
	//glTexCoord2f(0.0f, 0.0f);
	glVertex3f(max.x, min.y, max.z);
	//glTexCoord2f(0.0f, 1.0f);
	glVertex3f(max.x, max.y, max.z);
	//glTexCoord2f(1.0f, 1.0f);
	glVertex3f(min.x, max.y, max.z);
	//glTexCoord2f(1.0f, 0.0f);
	glVertex3f(min.x, min.y, max.z);
	glEnd();
	glDisable(GL_BLEND);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, 0);


	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//shade->unbind();

	glDisable(GL_CULL_FACE);
}
