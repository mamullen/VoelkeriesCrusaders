#ifndef MAP_H
#define MAP_H

#include "core.h"
#include "Shader.h"
#include "Texture.h"

class Map {
public:
	Map();
	void Draw(Shader* shader, Texture& m_pTrivialNormalMap, GLuint* photos);

	Texture colortex;
	Texture normaltex;
	
	Texture colorsand;
	Texture normalsand;
	Texture colortile;
	Texture normaltile;
	Texture colorshrine;
	Texture normalshrine;
	Texture colorcrus;
	Texture normalcrus;
	Texture colorvamp;
	Texture normalvamp;
	Texture colorgrass;
	Texture normalless;
	Texture colorless;

	Texture normalhalftile;
	Texture colorhalftile;
	Texture coloroldtile;
};

#endif