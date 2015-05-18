#include "ParticleSystem.h"
#include <assert.h>

ParticleEffect::ParticleEffect(unsigned int numParticles)
	: m_pCamera(NULL)
	, m_pParticleEmitter(NULL)
	, m_ColorInterpolator(Vector4(0, 0, 0, 0))
	, m_LocalToWorldMatrix(Matrix4x4())
	, m_TextureID(0)
	, m_Force(0, -9.81f, 0)
{
	Resize(numParticles);
}

ParticleEffect::~ParticleEffect()
{
	if (m_TextureID != 0)
	{
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = 0;
	}
}

void ParticleEffect::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

void ParticleEffect::SetParticleEmitter(ParticleEmitter* pEmitter)
{
	m_pParticleEmitter = pEmitter;
}

void ParticleEffect::SetColorInterplator(const ColorInterpolator& colors)
{
	m_ColorInterpolator = colors;
}

bool ParticleEffect::LoadTexture(const std::string& fileName)
{
	if (m_TextureID != 0)
	{
		glDeleteTextures(1, &m_TextureID);
	}

	m_TextureID = SOIL_load_OGL_texture(fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	return (m_TextureID != 0);
}

void ParticleEffect::RandomizeParticle(Particle& particle)
{
	particle.m_fAge = 0.0f;
	particle.m_fLifeTime = RandRange(3, 5);

	Vector3 unitVec = RandUnitVec();

	particle.m_Position = unitVec * 1.0f;
	particle.m_Velocity = unitVec * RandRange(10, 20);
}

void ParticleEffect::RandomizeParticles()
{
	for (unsigned int i = 0; i < m_Particles.size(); ++i)
	{
		RandomizeParticle(m_Particles[i]);
	}
}

void ParticleEffect::EmitParticle(Particle& particle)
{
	assert(m_pParticleEmitter != NULL);
	m_pParticleEmitter->EmitParticle(particle);
}

void ParticleEffect::EmitParticles()
{
	if (m_pParticleEmitter == NULL)
	{
		RandomizeParticles();
	}
	else
	{
		for (unsigned int i = 0; i < m_Particles.size(); ++i)
		{
			EmitParticle(m_Particles[i]);
		}
	}
}

void ParticleEffect::Update(float fDeltaTime)
{
	for (unsigned int i = 0; i < m_Particles.size(); ++i)
	{
		Particle& particle = m_Particles[i];

		particle.m_fAge += fDeltaTime;
		if (particle.m_fAge > particle.m_fLifeTime)
		{
			if (m_pParticleEmitter != NULL) EmitParticle(particle);
			else RandomizeParticle(particle);
		}

		float lifeRatio = saturate(particle.m_fAge / particle.m_fLifeTime);
		particle.m_Velocity += (m_Force * fDeltaTime);
		particle.m_Position += (particle.m_Velocity * fDeltaTime);
		particle.m_Color = m_ColorInterpolator.GetValue(lifeRatio);
		particle.m_fRotate = lerp(0.0f, 720.0f, lifeRatio);
		particle.m_fSize = lerp(5.0f, 0.0f, lifeRatio);
	}

	BuildVertexBuffer();
}

void ParticleEffect::BuildVertexBuffer()
{
}

void ParticleEffect::Render()
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glDepthMask(GL_FALSE);            // Disable depth writes
	glEnable(GL_BLEND);                 // Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // Type Of Blending To Perform
	glEnable(GL_TEXTURE_2D);            // Enable textures

	//glPushMatrix();
	//glMultMatrixf(glm::value_ptr(m_LocalToWorldMatrix));

	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Pos));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Tex0));
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Diffuse));

	glDrawArrays(GL_QUADS, 0, m_VertexBuffer.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopAttrib();

#if _DEBUG
	if (m_pParticleEmitter != NULL)
	{
		m_pParticleEmitter->DebugRender();
	}
#endif

	//glPopMatrix();

}

void ParticleEffect::Resize(unsigned int numParticles)
{
	m_Particles.resize(numParticles, Particle());
	m_VertexBuffer.resize(numParticles * 4, Vertex());
}
