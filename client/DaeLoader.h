////////////////////////////////////////
// DaeLoader.h
// - author: Wesley Lau
// loads daes
////////////////////////////////////////

#ifndef DAELOAD_H_
#define DAELOAD_H_

#include "core.h"
#include <assimp/ai_assert.h>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.h"
#include "vector3.h"
#include <map>
#include <iostream>
#include <vector>

struct Vertex {
	Vector3 position;
	Vector3 texture;
	Vector3 normal;
	Vertex() {}

	Vertex(Vector3& p, Vector3& t, Vector3& n) {
		position = p;
		texture = t;
		normal = n;
	}
};

class DaeLoader {
public:
	DaeLoader();
	~DaeLoader();
	DaeLoader(const std::string& filename);
	bool LoadAsset(const std::string& filename);
	void Render();
	void RenderNode(const aiNode* node);

private:
	bool initFromScene(const aiScene* pScene, const std::string& filename);
	void initMesh(unsigned int index, const aiMesh *paiMesh);
	bool initMaterials(const aiScene* pScene, const std::string& filename);
	void clear();

	struct MeshEntry {
		MeshEntry();
		~MeshEntry();
		void Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

	const aiScene* pScene;
};

#endif