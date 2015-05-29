////////////////////////////////////////
// MeshLoader.h
// - author: Wesley Lau
// loads daes
////////////////////////////////////////

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
private:
	#define NUM_BONES_PER_MESH 20
	#define INVALID_MATERIAL 0xFFFFFFFF

	struct VertexBoneData
	{
		unsigned int IDs[NUM_BONES_PER_MESH];
		float Weights[NUM_BONES_PER_MESH];

		VertexBoneData()
		{
			Reset();
		};

		void Reset()
		{
			memset(IDs, 0, sizeof(IDs));
			memset(Weights, 0, sizeof(Weights));
		}

		void AddBoneData(unsigned BoneID, float Weight);
	};
public:
	MeshLoader();
	~MeshLoader();
	MeshLoader(const char* filename);
	bool LoadAsset(const char* filename);
	void Render();

	void RenderMesh(const aiNode* node);
	void RenderBones(const aiNode* node);
	

private:
	void ApplyMaterial(const aiMaterial *material);

	const aiScene* m_Scene;
	SceneAnimator* mAnimator;
	bool r_Skeleton;
	double currentTime;
};

#endif