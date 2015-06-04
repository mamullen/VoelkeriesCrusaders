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
	CachedVertex() { cached = false; }
	aiVector3D vec;
	bool cached;
};

class MeshLoader {
private:
	bool m_EnforceNoBones;

	bool LoadAsset(const char* filename);
	void ApplyMaterial(const aiMaterial *material);

	void RenderMesh(const aiNode* node);
	void RenderBones(const aiNode* node);
public:
	MeshLoader();
	~MeshLoader();
	MeshLoader(const char* filename);
	
	void Render();
	void DisableBones()		{ m_EnforceNoBones = true; }
	void UpdateAnimation();
	void ChangeAnimation(unsigned int index);
	void SetLeftHand(const aiMatrix4x4& LeftHandPosTrafo) { m_LeftHandPosTrafo = new aiMatrix4x4(LeftHandPosTrafo); }
	aiMatrix4x4* GetLeftHand() { return m_LeftHandPosTrafo; }
	void IsEquippedWeapon(aiMatrix4x4* PosTrafo);

private:
	const aiScene* m_Scene;
	SceneAnimator* mAnimator;

	aiVector3D sceneCenter, sceneMin, sceneMax;
	void getBoundingBox(aiVector3D* min, aiVector3D* max);
	void getBoundingBoxForNode(const aiNode* node, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);
	aiMatrix4x4 *m_LeftHandPosTrafo;

	double a_CurrentTime;
	double a_LastPlaying;
	bool a_IsWeapon;
};

#endif