#ifndef EMITTER_H
#define EMITTER_H

#include "ParticleSystem.h"

class SphereEmitter : public ParticleEmitter
{
public:
	SphereEmitter();
	virtual void EmitParticle(Particle& particle);
	virtual void DebugRender();

	float MinimumRadius;
	float MaximumRadius;

	float MinInclination;
	float MaxInclination;

	float MinAzimuth;
	float MaxAzimuth;

	float MinSpeed;
	float MaxSpeed;

	float MinLifetime;
	float MaxLifetime;

	Vector3 Origin;

private:
	void RenderSphere(Vector4 color, float fRadius);
};

class CubeEmitter : public ParticleEmitter
{
public:
	CubeEmitter();

	virtual void EmitParticle(Particle& particle);
	virtual void DebugRender();

	float MinWidth;
	float MaxWidth;

	float MinHeight;
	float MaxHeight;

	float MinDepth;
	float MaxDepth;

	float MinSpeed;
	float MaxSpeed;

	float MinLifetime;
	float MaxLifetime;

	Vector3 Origin;
};

#endif