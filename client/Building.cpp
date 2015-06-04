#include "Building.h"

void Building::drawObj(){
	glEnable(GL_CULL_FACE);
	shade->bind();
	Vector3 min = *getMin();
	Vector3 max = *getMax();
	//glDisable(GL_CULL_FACE);
	//glColor3f(4.0f, 1.f, 1.f);
	int texture_location = glGetUniformLocation(shade->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	tex.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);

	int normal_location = glGetUniformLocation(shade->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	norm.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);
	//draw ceiling
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(min.x, max.y, min.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(min.x, max.y, max.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(max.x, max.y, max.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(max.x, max.y, min.z);

	//draw bottom
	//glNormal3f(0, 1, 0);
	//glVertex3f(max.x, min.y, min.z);
	//glVertex3f(max.x, min.y, max.z);
	//glVertex3f(min.x, min.y, max.z);
	//glVertex3f(min.x, min.y, min.z);

	//right
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(max.x, max.y, min.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(max.x, max.y, max.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(max.x, min.y, max.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(max.x, min.y, min.z);
	
	//left
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(min.x, min.y, min.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(min.x, min.y, max.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(min.x, max.y, max.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(min.x, max.y, min.z);

	//front
	
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(max.x, max.y, min.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(max.x, min.y, min.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(min.x, min.y, min.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(min.x, max.y, min.z);
	

	//back
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(max.x, min.y, max.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(max.x, max.y, max.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(min.x, max.y, max.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(min.x, min.y, max.z);
	glEnd();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	shade->unbind();
	
	glDisable(GL_CULL_FACE);
}