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

enum AnimationIndices { a_IDLE, a_RUNFORWARD, a_COMBOATTACK, a_STRAFELEFT, a_STRAFEFORWARDLEFT, a_STRAFERIGHT, a_STRAFEFORWARDRIGHT, a_WALKBACK, a_RUNMELEE };

struct CachedVertex {
	CachedVertex() { cached = false; }
	aiVector3D vec;
	bool cached;
};

struct MeshEntry {
	MeshEntry();
	~MeshEntry();
	void Init(const std::vector<Vector3>& CachedPositions, const std::vector<Vector3>& CachedNormals, const std::vector<unsigned int>& Indices);

	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;
};

class MeshLoader {
private:
	bool m_EnforceNoBones;

	bool LoadAsset(const char* filename);
	void ApplyMaterial(const aiMaterial *material); 
	void CalculateBones(const aiNode* node);

	void RenderMesh(const aiNode* node);
public:
	MeshLoader();
	~MeshLoader();
	MeshLoader(const char* filename);
	MeshLoader(const char* filename, bool model);

	void LoadScene();
	void LoadMesh(unsigned int index, const aiMesh* mesh, std::vector<Vector3>& CachedPositions, std::vector<Vector3>& CachedNormals, std::vector<Vector2>& TexCoords, std::vector<unsigned int>& Indices);
	
	void Render();
	void DisableBones()		{ m_EnforceNoBones = true; }
	void UpdateAnimation();
	void ChangeAnimation(unsigned int index);
	bool HasAnimations() { return a_IsAnimated; };
	float animRate = 2.f;

private:
	const aiScene* m_Scene;
	SceneAnimator* mAnimator;

	aiVector3D sceneCenter, sceneMin, sceneMax;
	void getBoundingBox(aiVector3D* min, aiVector3D* max);
	void getBoundingBoxForNode(const aiNode* node, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);

	double a_CurrentTime;
	double a_LastPlaying;
	bool a_IsWeapon;
	bool a_LockIndex;
	bool a_IsAnimated;
	bool playable;

	std::vector<MeshEntry> m_Entries;
	std::vector<Vector3> CachedPositions;
	std::vector<Vector3> CachedNormals;
	std::vector<Vector2> TexCoords;
	std::vector<unsigned int> Indices;

	enum VB_TYPES {
		INDEX_BUFFER,
		POS_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		NUM_VBs
	};
	GLuint m_VAO;
	GLuint m_Buffers[NUM_VBs];
};

#endif