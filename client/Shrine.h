#ifndef SHRINE_H_
#define SHRINE_H_

#include "core.h"
#include "matrix34.h"
#include "Vector4.h"
#include "MeshLoader.h"
#include "objparser.h"
#include "GameObject.h"
#include <iostream>
#include "ConfigSettings.h"

//TODO: plz add to server as gameobject
class Shrine : GameObject {
public:
	Shrine(unsigned int id) : GameObject(id) {
		g_Model = new MeshLoader((char*)ConfigSettings::config->getValue("ShrineAnimationFilePath").c_str());
		g_Model->DisableBones();
	}

	void Draw() {
		g_Model->UpdateAnimation();
		glPushMatrix();
		glEnable(GL_LIGHTING);
		glTranslatef(0.0f, -1.0f, 0.0f);
		glScalef(50.0f, 50.0f, 50.0f);
		g_Model->Render();
		glDisable(GL_LIGHTING);
		glPopMatrix();
	}
};

#endif
