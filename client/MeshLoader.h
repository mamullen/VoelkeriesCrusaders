////////////////////////////////////////
// MeshLoader.h
// - author: Wesley Lau
// loads daes
////////////////////////////////////////

#ifndef MESHLOAD_H_
#define MESHLOAD_H_

#include "core.h"
#include <assimp/ai_assert.h>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "AnimEvaluator.h"
#include "SceneAnimator.h"
#include <map>
#include <iostream>
#include <vector>

class MeshLoader {
public:
	MeshLoader();
	~MeshLoader();
	MeshLoader(const char* filename);
	int LoadAsset(const char* filename);
	void Render();
	void RenderNode(const aiNode* node);

private:
	void get_bounding_box(aiVector3D* min, aiVector3D* max);
	void get_bounding_box_for_node(const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);
	void apply_material(const struct aiMaterial *mtl);

	const aiScene* scene;
	aiVector3D scene_min, scene_max, scene_center;
	SceneAnimator* mAnimator;
	double currentTime;
};

#endif