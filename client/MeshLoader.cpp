#include "MeshLoader.h"

#define INVALID_VALUE 0xFFFFFFFF
#define INVALID_MATERIAL 0xFFFFFFFF
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

double currentTime = 0;

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

MeshLoader::MeshEntry::MeshEntry() {
	VB = INVALID_VALUE;
	IB = INVALID_VALUE;
	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
}

MeshLoader::MeshEntry::~MeshEntry()
{
	if (VB != INVALID_VALUE)
	{
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_VALUE)
	{
		glDeleteBuffers(1, &IB);
	}
}

void MeshLoader::MeshEntry::Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	NumIndices = indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &indices[0], GL_STATIC_DRAW);
}

MeshLoader::MeshLoader() {

}

MeshLoader::~MeshLoader() {
	clear();
}

void MeshLoader::clear() {
	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		SAFE_DELETE(m_Textures[i]);
	}
}

MeshLoader::MeshLoader(const std::string& filename) {
	std::cout << "MeshLoader:: loading " << filename << std::endl;
	LoadAsset(filename);
	std::cout << "MeshLoader:: finished " << std::endl;
}

bool MeshLoader::LoadAsset(const std::string& filename) {
	clear();

	bool ret = false;
	Assimp::Importer Importer;

	pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (pScene) {
		ret = initFromScene(pScene, filename.c_str());
	}
	else {
		printf("Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());
	}

	return ret;
}

bool MeshLoader::initFromScene(const aiScene* pScene, const std::string& filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		initMesh(i, paiMesh);
	}

	return initMaterials(pScene, filename);
}

void MeshLoader::initMesh(unsigned int index, const aiMesh* paiMesh)
{
	m_Entries[index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(Vector3(pPos->x, pPos->y, pPos->z),
			Vector3(pTexCoord->x, pTexCoord->y, 0),
			Vector3(pNormal->x, pNormal->y, pNormal->z));

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		ai_assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_Entries[index].Init(Vertices, Indices);
}

bool MeshLoader::initMaterials(const aiScene* pScene, const std::string& filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_Textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}

		// Load a white texture in case the model does not include its own texture
		if (!m_Textures[i]) {
			m_Textures[i] = new Texture(GL_TEXTURE_2D, "../Content/white.png");

			Ret = m_Textures[i]->Load();
		}
	}

	return Ret;
}


void MeshLoader::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
