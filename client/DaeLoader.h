////////////////////////////////////////
// DaeLoader.h
// - author: Wesley Lau
// loads daes
////////////////////////////////////////

#ifndef DAELOAD_H_
#define DAELOAD_H_

#include "core.h"
#include "Animator.h"
#include <boost/tuple/tuple.hpp>
#include <map>
#include <iostream>
#include <vector>

using namespace AssimpView;

class DaeLoader {
public:
	DaeLoader();
	~DaeLoader();
	DaeLoader(const char* filename);
	int LoadAsset(const char* filename);
	void Render();
	void RenderNode(const aiNode* node);

private:
	void get_bounding_box(aiVector3D* min, aiVector3D* max);
	void get_bounding_box_for_node(const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);
	void apply_material(const aiMaterial *mtl);

	const aiScene* scene;
	aiVector3D scene_min, scene_max, scene_center;
	SceneAnimator* mAnimator;
};

#endif