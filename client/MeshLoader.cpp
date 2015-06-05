///////////
// lauwood
//

#include "MeshLoader.h"
#include <time.h>
#include <string>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

#define INVALID_MATERIAL 0xFFFFFFFF

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TEX_COORD_LOCATION 2

MeshEntry::MeshEntry() {
	NumIndices = 0;
	BaseVertex = 0;
	BaseIndex = 0;
	MaterialIndex = INVALID_MATERIAL;
}

MeshEntry::~MeshEntry() {
	NumIndices = 0;
}

void MeshEntry::Init(const std::vector<Vector3>& CachedPositions, const std::vector<Vector3>& CachedNormals, const std::vector<unsigned int>& Indices) {
	NumIndices = Indices.size();
}

MeshLoader::MeshLoader() {
	a_LastPlaying = 0;
	a_CurrentTime = 0;
	m_Scene = NULL;
	m_EnforceNoBones = false;
	a_IsWeapon = false;
	a_LockIndex = false;
	a_IsAnimated = false;
	playable = false;
	memset(m_Buffers, 0, sizeof(m_Buffers));
	m_VAO = 0;
}

MeshLoader::MeshLoader(const char* filename) {
	a_LastPlaying = 0;
	a_CurrentTime = 0;
	m_Scene = NULL;
	m_EnforceNoBones = false;
	a_IsWeapon = false;
	a_LockIndex = false;
	a_IsAnimated = false;
	playable = false;
	memset(m_Buffers, 0, sizeof(m_Buffers));
	m_VAO = 0;

	std::cout << "MeshLoader:: loading " << filename << std::endl;
	if (!LoadAsset(filename)) {
		std::cout << "Error loading file" << std::endl;
	};
	std::cout << "MeshLoader:: finished " << std::endl;
}

MeshLoader::MeshLoader(const char* filename, bool model) {
	a_LastPlaying = 0;
	a_CurrentTime = 0;
	m_EnforceNoBones = false;
	playable = model;

	std::cout << "MeshLoader:: loading " << filename << std::endl;
	if (!LoadAsset(filename)) {
		std::cout << "Error loading file" << std::endl;
	};
	std::cout << "MeshLoader:: finished " << std::endl;


}

MeshLoader::~MeshLoader() {
	if (m_Buffers[0] != 0) {
		glDeleteBuffers(sizeof(m_Buffers) / sizeof(m_Buffers[0]), m_Buffers);
	}
	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
	}
	delete(m_Scene);
}


void MeshLoader::getBoundingBox(aiVector3D* min, aiVector3D* max) {
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	getBoundingBoxForNode(m_Scene->mRootNode, min, max, &trafo);
}

void MeshLoader::getBoundingBoxForNode(const aiNode* node, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo) {
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &node->mTransformation);

	for (; n < node->mNumMeshes; ++n) {
		aiMesh* mesh = m_Scene->mMeshes[node->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {
			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < node->mNumChildren; ++n) {
		getBoundingBoxForNode(node->mChildren[n], min, max, trafo);
	}
	*trafo = prev;
}


bool MeshLoader::LoadAsset(const char* filename) {
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(sizeof(m_Buffers) / sizeof(m_Buffers[0]), m_Buffers);

	m_Scene = aiImportFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!m_Scene) {
		std::cout << "Model was not loaded correctly." << std::endl;
		exit(-1);
	} else {
		getBoundingBox(&sceneMin, &sceneMax);
		sceneCenter.x = (sceneMin.x + sceneMax.x) / 2.0f;
		sceneCenter.y = (sceneMin.y + sceneMax.y) / 2.0f;
		sceneCenter.z = (sceneMin.z + sceneMax.z) / 2.0f;
	}

#if DEBUG
	for (unsigned int i = 0; i < m_Scene->mNumMeshes; i++) {
		std::cout << "\t\t" << i << " mesh name: " << m_Scene->mMeshes[i]->mName.C_Str() << " (" << m_Scene->mMeshes[i]->mNumFaces << ")" << std::endl;
		std::cout << "\t\t bones: " << m_Scene->mMeshes[i]->mNumBones << std::endl;
	}
#endif

	if (m_Scene->HasAnimations()) {
		std::cout << filename << " has animations:" << std::endl;
		for (unsigned int i = 0; i < m_Scene->mNumAnimations; i++) {
			std::cout << "\t" << m_Scene->mAnimations[i]->mName.C_Str() << std::endl;
		}
		a_IsAnimated = true;
		mAnimator = new SceneAnimator(m_Scene);
	}

	LoadScene();

	return true;
}

void MeshLoader::LoadScene() {
	m_Entries.resize(m_Scene->mNumMeshes);

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		m_Entries[i].MaterialIndex = m_Scene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = m_Scene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;

		NumVertices += m_Scene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	CachedPositions.reserve(NumVertices);
	CachedNormals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Indices.reserve(NumIndices);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* mesh = m_Scene->mMeshes[i];
		LoadMesh(i, mesh, CachedPositions, CachedNormals, TexCoords, Indices);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CachedPositions[0]) * CachedPositions.size(), &CachedPositions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CachedNormals[0]) * CachedNormals.size(), &CachedNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

void MeshLoader::LoadMesh(unsigned int index, const aiMesh* mesh, std::vector<Vector3>& CachedPositions, std::vector<Vector3>& CachedNormals, std::vector<Vector2>& TexCoords, std::vector<unsigned int>& Indices) {
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		const Vector3 vertices = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		const Vector3 normals = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		const Vector2 tex = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		CachedPositions.push_back(vertices);
		CachedNormals.push_back(normals);
		TexCoords.push_back(tex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace* face = &mesh->mFaces[i];
		if (face->mNumIndices == 2) {
			Indices.push_back(face->mIndices[0]);
			Indices.push_back(face->mIndices[1]);
			Indices.push_back(0);
		} else if (face->mNumIndices == 3) {
			Indices.push_back(face->mIndices[0]);
			Indices.push_back(face->mIndices[1]);
			Indices.push_back(face->mIndices[2]);
		}
	}
}

void MeshLoader::ChangeAnimation(unsigned int index) {
	if (m_Scene->HasAnimations() && !a_LockIndex) {
		mAnimator->SetAnimIndex(index);
	}
}

void color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

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
	/*if ((AI_SUCCESS == aiGetMaterialIntegerArray(material, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);*/
	if (playable)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}

	//glEnable(GL_CULL_FACE);
}

void MeshLoader::RenderMesh(const aiNode* node) {
	glPushMatrix();
	glBindVertexArray(m_VAO);
	glScalef(0.05, 0.05, 0.05);
	glEnable(GL_NORMALIZE);
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		ApplyMaterial(m_Scene->mMaterials[m_Entries[i].MaterialIndex]);
		glDrawElementsBaseVertex(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex), m_Entries[i].BaseVertex);
	}

	glBindVertexArray(0);
	glPopMatrix();

	/*
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		const aiMesh* mesh = m_Scene->mMeshes[node->mMeshes[i]];
		std::vector<CachedVertex> CachedPosition(mesh->mNumVertices);
		std::vector<CachedVertex> CachedNormal(mesh->mNumVertices);
		if (mesh->HasBones() && !m_EnforceNoBones) {
			const std::vector<aiMatrix4x4>& boneMatrices = mAnimator->GetBoneMatrices(node, i);

			for (unsigned int a = 0; a < mesh->mNumBones; a++) {
				const aiBone *bone = mesh->mBones[a];
				const aiMatrix4x4& posTrafo = boneMatrices[a];
				aiMatrix3x3 normTrafo = aiMatrix3x3(posTrafo);
				for (unsigned int b = 0; b < bone->mNumWeights; b++)
				{
					const aiVertexWeight& weight = bone->mWeights[b];
					unsigned int vertexId = weight.mVertexId;
					const aiVector3D& srcPos = mesh->mVertices[vertexId];
					const aiVector3D& srcNorm = mesh->mNormals[vertexId];
					CachedPosition[vertexId].vec += weight.mWeight * (posTrafo * srcPos);
					CachedPosition[vertexId].cached = true;
					CachedNormal[vertexId].vec += weight.mWeight * (normTrafo * srcNorm);
					CachedNormal[vertexId].cached = true;
				}
			}
		}
		aiMatrix4x4 Mx;
		if (m_Scene->HasAnimations()) {
			Mx = mAnimator->GetLocalTransform(node);
		}
		else {
			Mx = node->mTransformation;
		}
		Mx.Transpose();

		glPushMatrix();
		glMultMatrixf((float*)&Mx);

		ApplyMaterial(m_Scene->mMaterials[mesh->mMaterialIndex]);
		glScalef(0.05, 0.05, 0.05);
		glEnable(GL_NORMALIZE);
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
			const aiFace* face = &mesh->mFaces[j];
			glBegin(GL_TRIANGLES);

			glScalef(0.1, 0.1, 0.1);

			if (face->mNumIndices == 3) {
				glNormal3fv(&mesh->mNormals[face->mIndices[0]].x);
				glVertex3fv(&mesh->mVertices[face->mIndices[0]].x);
				glNormal3fv(&mesh->mNormals[face->mIndices[1]].x);
				glVertex3fv(&mesh->mVertices[face->mIndices[1]].x);
				glNormal3fv(&mesh->mNormals[face->mIndices[2]].x);
				glVertex3fv(&mesh->mVertices[face->mIndices[2]].x);
			}
			
			for (unsigned int k = 0; k < face->mNumIndices; k++) {
				int v_index = face->mIndices[k];
				if (mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][v_index]);
				if (mesh->mNormals != NULL) {
					glNormal3fv(&mesh->mNormals[v_index].x);
					if (&CachedNormal[v_index].cached && !m_EnforceNoBones) {
						glNormal3fv(&CachedNormal[v_index].vec.x);
					} else {
						
					}
				}

				if (&CachedPosition[v_index].cached && !m_EnforceNoBones) {
					glVertex3fv(&CachedPosition[v_index].vec.x);
				} else {
					glVertex3fv(&mesh->mVertices[v_index].x);
				}
			}
			glEnd();
		}

		glPopMatrix();
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		RenderMesh(node->mChildren[i]);
	}*/
}

void MeshLoader::UpdateAnimation() {
	if (m_Scene->HasAnimations()) {
		a_CurrentTime += clock() *animRate / double(CLOCKS_PER_SEC) - a_LastPlaying;
		mAnimator->Calculate(a_CurrentTime);
		a_LockIndex = false;
		a_LastPlaying = a_CurrentTime;
	}
}

void MeshLoader::CalculateBones(const aiNode* node) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		const aiMesh* mesh = m_Scene->mMeshes[node->mMeshes[i]];
		if (mesh->HasBones() && !m_EnforceNoBones) {
			const std::vector<aiMatrix4x4>& boneMatrices = mAnimator->GetBoneMatrices(node, i);

			for (unsigned int a = 0; a < mesh->mNumBones; a++) {
				const aiBone *bone = mesh->mBones[a];
				const aiMatrix4x4& posTrafo = boneMatrices[a];
				aiMatrix3x3 normTrafo = aiMatrix3x3(posTrafo);
				for (unsigned int b = 0; b < bone->mNumWeights; b++)
				{
					const aiVertexWeight& weight = bone->mWeights[b];
					unsigned int vertexId = weight.mVertexId;
					const aiVector3D& srcPos = mesh->mVertices[vertexId];
					const aiVector3D& srcNorm = mesh->mNormals[vertexId];
					aiVector3D cpos;
					aiVector3D cnorm;
					cpos += weight.mWeight * (posTrafo * srcPos);
					cnorm += weight.mWeight * (normTrafo * srcNorm);
					CachedPositions[vertexId] = Vector3(cpos.x, cpos.y, cpos.z);
					CachedNormals[vertexId] = Vector3(cnorm.x, cnorm.y, cnorm.z);
				}
			}
		}
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		CalculateBones(node->mChildren[i]);
	}
}

void MeshLoader::Render() {
	//std::cout << sceneCenter.x << " " << sceneCenter.y << " " << sceneCenter.z << std::endl;
	glEnable(GL_LIGHTING);
	CalculateBones(m_Scene->mRootNode);
	RenderMesh(m_Scene->mRootNode);
	glDisable(GL_LIGHTING);
}
