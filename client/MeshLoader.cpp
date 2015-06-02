///////////
// lauwood
//

#include "MeshLoader.h"
#include <time.h>
#include <string>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

MeshLoader::MeshLoader() {
	std::cout << "No filename specified" << std::endl;
	a_LastPlaying = 0;
	a_CurrentTime = 0;
	m_Scene = NULL;
	m_EnforceNoBones = false;

}

MeshLoader::MeshLoader(const char* filename) {
	a_LastPlaying = 0;
	a_CurrentTime = 0;
	m_EnforceNoBones = false;

	std::cout << "MeshLoader:: loading " << filename << std::endl;
	if (!LoadAsset(filename)) {
		std::cout << "Error loading file" << std::endl;
	};
	std::cout << "MeshLoader:: finished " << std::endl;
}

MeshLoader::~MeshLoader() {
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
		mAnimator = new SceneAnimator(m_Scene);
	}
	return true;
}

void MeshLoader::ChangeAnimation(unsigned int index) {
	if (m_Scene->HasAnimations()) {
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
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(material, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}

void MeshLoader::RenderMesh(const aiNode* node) {
	// naive, put into VBO
	glPushMatrix();
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
			for (unsigned int k = 0; k < face->mNumIndices; k++) {
				int v_index = face->mIndices[k];
				if (mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][v_index]);
				if (mesh->mNormals != NULL) {
					if (&CachedNormal[v_index].cached && !m_EnforceNoBones) {
						glNormal3fv(&CachedNormal[v_index].vec.x);
					} else {
						glNormal3fv(&mesh->mNormals[v_index].x);	
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
	}
	glPopMatrix();
}

void MeshLoader::UpdateAnimation() {
	if (m_Scene->HasAnimations()) {
		a_CurrentTime += clock() / double(CLOCKS_PER_SEC) - a_LastPlaying;
		double time = a_CurrentTime;
		aiAnimation* mAnim = mAnimator->CurrentAnim();
		if (mAnim && mAnim->mDuration > 0.0) {
			double tps = 25.f;
			time = fmod(time, mAnim->mDuration / tps);
		}
		mAnimator->Calculate(time);
		a_LastPlaying = a_CurrentTime;
	}
}

void MeshLoader::Render() {
	//std::cout << sceneCenter.x << " " << sceneCenter.y << " " << sceneCenter.z << std::endl;
	glEnable(GL_LIGHTING);
	RenderMesh(m_Scene->mRootNode);
	glDisable(GL_LIGHTING);
}
