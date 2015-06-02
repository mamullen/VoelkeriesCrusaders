#include "Map.h"

void Map::Draw(Shader* p_bumpShade, Texture& colortex, Texture& normaltex, Texture& colorslant, Texture& normalslant, Texture& m_pTrivialNormalMap, GLuint* photos) {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//p_bumpShade->bind();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_TEXTURE_2D);
	int texture_location = glGetUniformLocation(p_bumpShade->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	colortex.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	int normal_location = glGetUniformLocation(p_bumpShade->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normaltex.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-240, -1, -240);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-240, -1, -90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-80, -1, -90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-80, -1, -240);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-80, -1, -240);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-80, -1, -90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(80, -1, -90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(80, -1, -240);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(80, -1, -240);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(80, -1, -90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(240, -1, -90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(240, -1, -240);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-240, -1, 90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-240, -1, 240);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-90, -1, 240);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-90, -1, 90);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-90, -1, 90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-90, -1, 240);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(90, -1, 240);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(90, -1, 90);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(90, -1, 90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(90, -1, 240);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(240, -1, 240);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(240, -1, 90);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-240, -1, -90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-240, -1, 90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-100, -1, 90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100, -1, -90);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(100, -1, -90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100, -1, 90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(240, -1, 90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(240, -1, -90);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-75, -8, -60);//
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-75, -8, 60);//
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(75, -8, 60);////
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(75, -8, -60);////

	glEnd();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	//p_bumpShade->unbind();

	//p_bumpShade->bind();

	glEnable(GL_TEXTURE_2D);
	texture_location = glGetUniformLocation(p_bumpShade->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	colorslant.Bind(GL_TEXTURE0);

	glEnable(GL_TEXTURE_2D);
	normal_location = glGetUniformLocation(p_bumpShade->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normalslant.Bind(GL_TEXTURE1);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(100, -1, 90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(75, -8, 60);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-75, -8, 60);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100, -1, 90);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100, -1, -90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-75, -8, -60);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(75, -8, -60);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100, -1, -90);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100, -1, 90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-75, -8, 60);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-75, -8, -60);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100, -1, -90);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(100, -1, -90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(75, -8, -60);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(75, -8, 60);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100, -1, 90);

	glEnd();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	//p_bumpShade->unbind();

	//p_bumpShade->bind();

	//glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	normal_location = glGetUniformLocation(p_bumpShade->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	m_pTrivialNormalMap.Bind(GL_TEXTURE1);

	glEnable(GL_TEXTURE_2D);
	texture_location = glGetUniformLocation(p_bumpShade->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, photos[0]);
	glBegin(GL_QUADS);
	//front
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-260, 200, -260);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-260, -60, -260);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(260, -60, -260);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(260, 200, -260);
	glEnd();
	//back
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, photos[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(260, 200, 260);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(260, -60, 260);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-260, -60, 260);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-260, 200, 260);
	glEnd();


	//right
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, photos[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(260, 200, -260);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(260, -60, -260);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(260, -60, 260);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(260, 200, 260);
	glEnd();

	//left
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, photos[3]);
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-260, 200, 260);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-260, -60, 260);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-260, -60, -260);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-260, 200, -260);
	glEnd();
	//shader.unbind();

	//Top
	//glDisable(GL_CULL_FACE);
	//glNormal3f(0, -1, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, photos[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-260, 200, -260);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(260, 200, -260);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(260, 200, 260);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-260, 200, 260);
	//glDisable(GL_TEXTURE_2D);
	glEnd();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	//p_bumpShade->unbind();
}