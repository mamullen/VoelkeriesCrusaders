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

struct CachedVertex {
	CachedVertex() : vec(), cached(false) {}
	aiVector3D vec;
	bool cached;
};

class MeshLoader {
private:
	bool LoadAsset(const char* filename);
	void ApplyMaterial(const aiMaterial *material);

	void RenderMesh(const aiNode* node);
	void RenderBones(const aiNode* node);
public:
	MeshLoader();
	~MeshLoader();
	MeshLoader(const char* filename);
	
	void Render();
	void UpdateAnimation();
	void ChangeAnimation(unsigned int index);

private:
	const aiScene* m_Scene;
	SceneAnimator* mAnimator;


	aiVector3D sceneCenter, sceneMin, sceneMax;
	void getBoundingBox(aiVector3D* min, aiVector3D* max);
	void getBoundingBoxForNode(const aiNode* node, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);

	double a_CurrentTime;
	double a_LastPlaying;
};

#endif