#include "MeshLoader.h"
#include <time.h>
#include <string>

void MeshLoader::VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
	for (unsigned int i = 0; i < (sizeof(IDs)/sizeof(IDs[0])); i++) {
		if (Weights[i] == 0.0) {
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}

	// should never get here - more bones than we have space for
	//assert(0);
}

MeshLoader::MeshLoader() {
	std::cout << "No filename specified" << std::endl;
	currentTime = 0;
	r_Skeleton = false;
	m_Scene = NULL;

}

MeshLoader::MeshLoader(const char* filename) {
	currentTime = 0;
	r_Skeleton = true;

	std::cout << "MeshLoader:: loading " << filename << std::endl;
	if (!LoadAsset(filename)) {
		std::cout << "Error loading file" << std::endl;
	};
	std::cout << "MeshLoader:: finished " << std::endl;
}

MeshLoader::~MeshLoader() {
}

bool MeshLoader::LoadAsset(const char* filename) {
	bool ret;
	m_Scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);

	if (m_Scene) {
		m_GlobalInverseTransform = m_Scene->mRootNode->mTransformation;
		m_GlobalInverseTransform;
		ret = InitFromScene(m_Scene, filename);
		//ret = true;
	}

	std::cout << "\t number of meshes: " << m_Scene->mNumMeshes << std::endl;

	for (unsigned int i = 0; i < m_Scene->mNumMeshes; i++) {
		std::cout << "\t\t" << i << " mesh name: " << m_Scene->mMeshes[i]->mName.C_Str() << " (" << m_Scene->mMeshes[i]->mNumFaces << ")" << std::endl;
		std::cout << "\t\t bones: " << m_Scene->mMeshes[i]->mNumBones << std::endl;
	}

	if (m_Scene->HasAnimations()) {
		std::cout << filename << " has animations:" << std::endl;
		for (unsigned int i = 0; i < m_Scene->mNumAnimations; i++) {
			std::cout << "\t" << m_Scene->mAnimations[i]->mName.C_Str() << std::endl;
		}
		mAnimator = new SceneAnimator(m_Scene);
	}
	return ret;
}

bool MeshLoader::InitFromScene(const aiScene* pScene, const char* filename) {
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	vector<Vector3> Positions;
	vector<Vector3> Normals;
	vector<Vector2> TexCoords;
	vector<VertexBoneData> Bones;
	vector<unsigned int> Indices;

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	// Count the number of vertices and indices
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	Positions.reserve(NumVertices);
	Normals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Bones.resize(NumVertices);
	Indices.reserve(NumIndices);

	// InitMesh one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
	}

	/*if (!InitMaterials(pScene, Filename)) {
		return false;
	}*/

	return true;
}

void MeshLoader::InitMesh(unsigned int MeshIndex,
	const aiMesh* paiMesh,
	vector<Vector3>& Positions,
	vector<Vector3>& Normals,
	vector<Vector2>& TexCoords,
	vector<VertexBoneData>& Bones,
	vector<unsigned int>& Indices) {
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(Vector3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(Vector3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(Vector2(pTexCoord->x, pTexCoord->y));
	}

	//LoadBones(MeshIndex, paiMesh, Bones);

	// Populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 2);
		//if (Face.mNumIndices != 3) {
			//std::cout << Face.mNumIndices << std::endl;
		//}
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}

void MeshLoader::LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones) {
	for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
		unsigned int BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			// Allocate an index for a new bone
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
			m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
			unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

void color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

////////////////////////////////////////////////////////////////////////////////

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void MeshLoader::ApplyMaterial(const struct aiMaterial *material)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max);
	if (ret1 == AI_SUCCESS) {
		max = 1;
		ret2 = aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if (ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
		else
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(material, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(material, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}

void MeshLoader::RenderSkeleton() {
	glPushMatrix();
	glPopMatrix();
}

void MeshLoader::RenderNode() {
}


void MeshLoader::Render() {
	/*static double lastPlaying = 0.;

	ai_assert(mAnimator);

	currentTime += clock() / double(CLOCKS_PER_SEC) - lastPlaying;
	double time = currentTime;
	aiAnimation* mAnim = mAnimator->CurrentAnim();
	if (mAnim && mAnim->mDuration > 0.0) {
		double tps = mAnim->mTicksPerSecond ? mAnim->mTicksPerSecond : 25.f;
		time = fmod(time, mAnim->mDuration / tps);
	}
	mAnimator->Calculate(time);
	lastPlaying = currentTime;*/

	glEnable(GL_LIGHTING);
	RenderNode();

	if (r_Skeleton) {
		RenderSkeleton();
	}
	glDisable(GL_LIGHTING);
}
