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

class Shrine {
private:
	MeshLoader *s_Model;
public:
	Shrine() {
		s_Model = new MeshLoader((char*)ConfigSettings::config->getValue("ShrineAnimationFilePath").c_str());
	}

	void Draw() {
		s_Model->Render();
	}
};

#endif
