#ifndef PSYS_H_
#define PSYS_H_

#include "core.h"
#include "camera.h"
#include "Interpolator.h"
#include "Random.h"
#include "vector2.h"
#include "vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include <vector>

struct Particle
{
	Particle() : m_Position(0), m_Velocity(0), m_Color(0), m_fRotate(0), m_fSize(0), m_fAge(0), m_fLifeTime(0) {}

	Vector3 m_Position;
	Vector3 m_Velocity;
	Vector4 m_Color;
	float m_fRotate;
	float m_fSize;
	float m_fAge;
	float m_fLifeTime;
};

typedef std::vector<Particle> ParticleBuffer;

class ParticleEmitter {
public:
	virtual ~ParticleEmitter() {}
	virtual void EmitParticle(Particle& particle) = 0;

	virtual void DebugRender() {}
};


class ParticleEffect
{
public:
	struct Vertex {
		Vertex() : m_Pos(0), m_Diffuse(0), m_Tex0(0) {}
		Vector3 m_Pos;
		Vector4 m_Diffuse;
		Vector2 m_Tex0;
	};

	typedef std::vector<Vertex> VertexBuffer;
	typedef Interpolator<Vector4> ColorInterpolator;

	ParticleEffect(unsigned int numParticles = 0);
	virtual ~ParticleEffect();

	void SetCamera(Camera* pCamera);
	void SetParticleEmitter(ParticleEmitter* pEmitter);
	void SetColorInterplator(const ColorInterpolator& colors);

	// Test method to randomize the particles in an interesting way
	void RandomizeParticles();
	void EmitParticles();

	virtual void Update(float fDeltaTime);
	virtual void Render();

	bool LoadTexture(const std::string& fileName);

	// Resize the particle buffer with numParticles
	void Resize(unsigned int numParticles);

protected:
	void RandomizeParticle(Particle& particle);
	void EmitParticle(Particle& particle);
public:
	// Build the vertex buffer from the particle buffer
	void BuildVertexBuffer();
private:
	Camera*             m_pCamera;
	ParticleEmitter*    m_pParticleEmitter;
	ColorInterpolator   m_ColorInterpolator;

	ParticleBuffer      m_Particles;
	VertexBuffer        m_VertexBuffer;
	Matrix4x4	        m_LocalToWorldMatrix;
	GLuint              m_TextureID;

	// Apply this force to every particle in the effect
	Vector3				m_Force;
};

#endif