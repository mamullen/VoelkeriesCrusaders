#ifndef MAP_H
#define MAP_H

#include "core.h"
#include "Shader.h"
#include "Texture.h"

class Map {
public:
	void Draw(Shader& shader, Texture& colortex, Texture& normaltex, Texture& colorslant, Texture& normalslant, Texture& m_pTrivialNormalMap, GLuint* photos);
};

#endif