////////////
// lauwood
//

#ifndef MESHLOAD_H_
#define MESHLOAD_H_

#include "core.h"
#include <assimp/ai_assert.h>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "AnimEvaluator.h"
#include "SceneAnimator.h"
#include "Texture.h"
#include "Matrix4x4.h"
#include "vector3.h"
#include "vector2.h"
#include <map>
#include <iostream>
#include <vector>

class MeshLoader {
public:
	MeshLoader();
	~MeshLoader();
	MeshLoader(const char* filename);
	bool LoadAsset(const char* filename);
	void Render();
	void UpdateAnimation();
	void ChangeAnimation(unsigned int index);

	void RenderMesh(const aiNode* node);
	void RenderBones(const aiNode* node);
	

private:
	void ApplyMaterial(const aiMaterial *material);

	const aiScene* m_Scene;
	SceneAnimator* mAnimator;

	double currentTime;
};

#endif