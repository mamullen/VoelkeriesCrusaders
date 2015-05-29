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
	bool InitFromScene(const aiScene* scene, const char* filename);

	void InitMesh(unsigned int MeshIndex,
		const aiMesh* paiMesh,
		vector<Vector3>& Positions,
		vector<Vector3>& Normals,
		vector<Vector2>& TexCoords,
		vector<VertexBoneData>& Bones,
		vector<unsigned int>& Indices);

	void Render();

	void RenderNode();
	void RenderSkeleton();
	

private:
	struct BoneInfo {
		Matrix4x4 BoneOffset;
		Matrix4x4 FinalTransformation;

		BoneInfo()
		{
			BoneOffset.SetZero();
			FinalTransformation.SetZero();
		}
	};


	struct MeshEntry {
		MeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int MaterialIndex;
	};

	void LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones);
	void ApplyMaterial(const aiMaterial *material);

	const aiScene* m_Scene;

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

	std::map<std::string, unsigned int> m_BoneMapping; // maps a bone name to its index
	unsigned int m_NumBones;
	std::vector<BoneInfo> m_BoneInfo;

	Matrix4x4 m_GlobalInverseTransform;
	SceneAnimator* mAnimator;
	double currentTime;

	bool r_Skeleton;
};

#endif