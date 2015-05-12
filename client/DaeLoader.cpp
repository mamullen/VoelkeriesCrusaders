#include "DaeLoader.h"

#define INVALID_VALUE 0xFFFFFFFF
#define INVALID_MATERIAL 0xFFFFFFFF
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

double currentTime = 0;

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

DaeLoader::MeshEntry::MeshEntry() {
	VB = INVALID_VALUE;
	IB = INVALID_VALUE;
	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
}

DaeLoader::MeshEntry::~MeshEntry()
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

void DaeLoader::MeshEntry::Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	NumIndices = indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &indices[0], GL_STATIC_DRAW);
}

DaeLoader::DaeLoader() {
	
}

DaeLoader::~DaeLoader() {
	clear();
}

void DaeLoader::clear() {
	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		SAFE_DELETE(m_Textures[i]);
	}
}

DaeLoader::DaeLoader(const std::string& filename) {
	std::cout << "DaeLoader:: loading " << filename << std::endl;
	LoadAsset(filename);
	std::cout << "DaeLoader:: finished " << std::endl;
}

bool DaeLoader::LoadAsset(const std::string& filename) {
	clear();

	bool ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (pScene) {
		ret = initFromScene(pScene, filename.c_str());
	}
	else {
		printf("Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());
	}

	return ret;
}

bool DaeLoader::initFromScene(const aiScene* pScene, const std::string& filename)
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

void DaeLoader::initMesh(unsigned int index, const aiMesh* paiMesh)
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

bool DaeLoader::initMaterials(const aiScene* pScene, const std::string& filename)
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


void DaeLoader::Render()
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

void DaeLoader::get_bounding_box(aiVector3D* min, aiVector3D* max) {
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
}

void DaeLoader::get_bounding_box_for_node(const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo) {
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
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

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n], min, max, trafo);
	}
	*trafo = prev;
}

////////////////////////////////////////////////////////////////////////////////

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

void DaeLoader::apply_material(const aiMaterial *mtl)
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
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if (ret1 == AI_SUCCESS) {
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
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
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}

void DaeLoader::RenderNode(const aiNode* node) {
	aiMatrix4x4 aiMe = mAnimator->GetLocalTransform(node);
	aiMe.Transpose();

	glPushMatrix();
	glMultMatrixf((float*)&aiMe);

	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		apply_material(scene->mMaterials[mesh->mMaterialIndex]);

		for (unsigned j = 0; j < mesh->mNumFaces; ++j) {
			const aiFace* face = &mesh->mFaces[j];

			glBegin(GL_TRIANGLES);

			for (unsigned int k = 0; k < face->mNumIndices; k++) {
				int index = face->mIndices[k];
				if (mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if (mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		RenderNode(node->mChildren[i]);
	}
	glPopMatrix();
}
/*
void DaeLoader::Render() {
	static double lastPlaying = 0.;

	ai_assert(mAnimator);

	currentTime += glfwGetTime() - lastPlaying;
	double time = currentTime;
	aiAnimation* mAnim = mAnimator->CurrentAnim();
	if (mAnim && mAnim->mDuration > 0.0) {
		double tps = mAnim->mTicksPerSecond ? mAnim->mTicksPerSecond : 25.f;
		time = fmod(time, mAnim->mDuration / tps);
	}
	mAnimator->Calculate(time);
	lastPlaying = currentTime;

	float tmp = 2;
	glScalef(tmp, tmp, tmp);
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);

	glEnable(GL_LIGHTING);
	RenderNode(scene->mRootNode);
	glDisable(GL_LIGHTING);
}
*/