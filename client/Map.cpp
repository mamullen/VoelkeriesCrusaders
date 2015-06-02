#include "Map.h"

Map::Map()
{
	colortex.initTexture(GL_TEXTURE_2D, "ppm/bleh_C.png");
	normaltex.initTexture(GL_TEXTURE_2D, "ppm/bleh_N.png");

	colortex.Load();
	normaltex.Load();


	colorsand.initTexture(GL_TEXTURE_2D, "ppm/age.png");
	normalsand.initTexture(GL_TEXTURE_2D, "ppm/tile_nm.png");

	colorsand.Load();
	normalsand.Load();

	colortile.initTexture(GL_TEXTURE_2D, "ppm/tile.png");
	normaltile.initTexture(GL_TEXTURE_2D, "ppm/tile_nm.png");

	colortile.Load();
	normaltile.Load();

	colorhalftile.initTexture(GL_TEXTURE_2D, "ppm/bleh_C2.png");
	normalhalftile.initTexture(GL_TEXTURE_2D, "ppm/bleh_N2.png");

	colorhalftile.Load();
	normalhalftile.Load();

	colorshrine.initTexture(GL_TEXTURE_2D, "ppm/shrine.png");
	normalshrine.initTexture(GL_TEXTURE_2D, "ppm/shrine_nm.png");

	colorshrine.Load();
	normalshrine.Load();

	colorvamp.initTexture(GL_TEXTURE_2D, "ppm/vamp.png");
	normalvamp.initTexture(GL_TEXTURE_2D, "ppm/vamp_nm.png");

	colorvamp.Load();
	normalvamp.Load();

	colorcrus.initTexture(GL_TEXTURE_2D, "ppm/crusader.png");
	normalcrus.initTexture(GL_TEXTURE_2D, "ppm/crusader_nm.png");

	colorcrus.Load();
	normalcrus.Load();

	coloroldtile.initTexture(GL_TEXTURE_2D, "ppm/tile_2.png");
	coloroldtile.Load();

	colorgrass.initTexture(GL_TEXTURE_2D, "ppm/bleh_C3.png");
	colorgrass.Load();

	colorless.initTexture(GL_TEXTURE_2D, "ppm/parttile.png");
	normalless.initTexture(GL_TEXTURE_2D, "ppm/parttile_nm.png");

	colorless.Load();
	normalless.Load();
}


void Map::Draw(Shader* shader, Texture& m_pTrivialNormalMap, GLuint* photos) {

	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	shader->bind();

	glEnable(GL_TEXTURE_2D);
	int texture_location = glGetUniformLocation(shader->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	colortex.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);

	int normal_location = glGetUniformLocation(shader->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normaltex.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);




	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(100, -1, -90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(100, -1, 90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(240, -1, 90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(240, -1, -90);



	glEnd();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//shader.unbind();

	glEnable(GL_TEXTURE_2D);
	texture_location = glGetUniformLocation(shader->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	colorless.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);

	normal_location = glGetUniformLocation(shader->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normalless.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(80, -1, -240);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(80, -1, -90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(240, -1, -90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(240, -1, -240);

	glEnd();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//shader.unbind();

	//shader.unbind();
	glEnable(GL_TEXTURE_2D);
	texture_location = glGetUniformLocation(shader->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	colorgrass.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);

	normal_location = glGetUniformLocation(shader->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normalhalftile.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100, -1, -240);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100, -1, -90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(80, -1, -90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(80, -1, -240);
	glEnd();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);


	//HERE 
	texture_location = glGetUniformLocation(shader->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	colorhalftile.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);

	normal_location = glGetUniformLocation(shader->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normalhalftile.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(90, -1, 90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(90, -1, 240);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(240, -1, 240);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(240, -1, 90);

	glEnd();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//shader.unbind();


	//shader.unbind();

	//shader.bind();
	texture_location = glGetUniformLocation(shader->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	colortile.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);

	normal_location = glGetUniformLocation(shader->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normaltile.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);
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



	glEnd();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//shader.unbind();


	//shader.bind();
	texture_location = glGetUniformLocation(shader->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	coloroldtile.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);

	normal_location = glGetUniformLocation(shader->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normaltile.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-240, -1, -90);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-240, -1, 90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-100, -1, 90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100, -1, -90);
	glEnd();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//shader.unbind();



	//here

	//shader.bind();

	texture_location = glGetUniformLocation(shader->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	colorshrine.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);

	normal_location = glGetUniformLocation(shader->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normalshrine.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100, -1, -90);//
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-100, -1, 90);//
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(100, -1, 90);////
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(100, -1, -90);////

	glEnd();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//shader.unbind();




	//shader.bind();

	texture_location = glGetUniformLocation(shader->id(), "color_texture");
	glUniform1i(texture_location, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	colorsand.Bind(GL_TEXTURE0);

	//glActiveTexture(GL_TEXTURE1);

	normal_location = glGetUniformLocation(shader->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	normalsand.Bind(GL_TEXTURE1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-240, -1, -240);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-240, -1, -90);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-100, -1, -90);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-100, -1, -240);

	glEnd();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);


	//shader.unbind();
	//shader.bind();

	//glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	normal_location = glGetUniformLocation(shader->id(), "normal_texture");
	glUniform1i(normal_location, 1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normal_texture);
	m_pTrivialNormalMap.Bind(GL_TEXTURE1);

	glEnable(GL_TEXTURE_2D);
	texture_location = glGetUniformLocation(shader->id(), "color_texture");
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
	glVertex3f(-340, 230, -340);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-340, -110, -340);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(340, -110, -340);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(340, 230, -340);
	glEnd();
	//back
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, photos[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(340, 230, 340);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(340, -110, 340);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-340, -110, 340);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-340, 230, 340);
	glEnd();


	//right
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, photos[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(340, 230, -340);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(340, -110, -340);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(340, -110, 340);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(340, 230, 340);
	glEnd();

	//left
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, photos[3]);
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-340, 230, 340);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-340, -110, 340);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-340, -110, -340);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-340, 230, -340);
	glEnd();
	//shader.unbind();

	//Top
	//glDisable(GL_CULL_FACE);
	//glNormal3f(0, -1, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, photos[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-340, 230, -340);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(340, 230, -340);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(340, 230, 340);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-340, 230, 340);
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