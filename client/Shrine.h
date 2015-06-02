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

//TODO: plz make gameobject
class Shrine {
private:
	MeshLoader *s_Model;
public:
	Shrine()  {
		s_Model = new MeshLoader((char*)ConfigSettings::config->getValue("ShrineAnimationFilePath").c_str());
		s_Model->DisableBones();
	}

	void Draw() {
		s_Model->UpdateAnimation();
		glPushMatrix();
		glEnable(GL_LIGHTING);
		glTranslatef(100.0f, 0.0f, 100.0f);
		glScalef(50.0f, 50.0f, 50.0f);
		s_Model->Render();
		glDisable(GL_LIGHTING);
		glPopMatrix();
	}
};

#endif
