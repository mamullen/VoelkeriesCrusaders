#include "Emitter.h"

CubeEmitter::CubeEmitter()
	: MinWidth(-1)
	, MaxWidth(1)
	, MinHeight(-1)
	, MaxHeight(1)
	, MinDepth(-1)
	, MaxDepth(1)
	, MinSpeed(10)
	, MaxSpeed(20)
	, MinLifetime(3)
	, MaxLifetime(5)
	, Origin(0)
{}

void CubeEmitter::EmitParticle(Particle& particle)
{
	float X = RandRange(MinWidth, MaxWidth);
	float Y = RandRange(MinHeight, MaxHeight);
	float Z = RandRange(MinDepth, MaxDepth);

	float lifetime = RandRange(MinLifetime, MaxLifetime);
	float speed = RandRange(MinSpeed, MaxSpeed);

	Vector3 vector(X, Y, Z);

	particle.m_Position = vector + Origin;
	particle.m_Velocity = vector.Normalize() * speed;

	particle.m_fLifeTime = lifetime;
	particle.m_fAge = 0;
}

void CubeEmitter::DebugRender()
{
	glTranslatef(Origin.x, Origin.y, Origin.z);

	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

	//glutWireCube(MaxWidth * 2);

	glTranslatef(-Origin.x, -Origin.y, -Origin.z);
}

SphereEmitter::SphereEmitter()
	: MinimumRadius(0)
	, MaximumRadius(1)
	, MinInclination(0)
	, MaxInclination(180)
	, MinAzimuth(0)
	, MaxAzimuth(360)
	, MinSpeed(10)
	, MaxSpeed(20)
	, MinLifetime(3)
	, MaxLifetime(5)
	, Origin(0)
{}

void SphereEmitter::EmitParticle(Particle& particle)
{
	float inclination = toRadians(RandRange(MinInclination, MaxInclination));
	float azimuth = toRadians(RandRange(MinAzimuth, MaxAzimuth));

	float radius = RandRange(MinimumRadius, MaximumRadius);
	float speed = RandRange(MinSpeed, MaxSpeed);
	float lifetime = RandRange(MinLifetime, MaxLifetime);

	float sInclination = sinf(inclination);

	float X = sInclination * cosf(azimuth);
	float Y = sInclination * sinf(azimuth);
	float Z = cosf(inclination);

	Vector3 vector(X, Y, Z);

	particle.m_Position = (vector * radius) + Origin;
	particle.m_Velocity = vector * speed;

	particle.m_fLifeTime = lifetime;
	particle.m_fAge = 0;
}

void SphereEmitter::RenderSphere(Vector4 color, float fRadius)
{
	float X, Y, Z, inc, azi;

	glColor4f(color.x, color.y, color.z, color.w);

	glPointSize(2.0f);
	glBegin(GL_POINTS);

	for (float azimuth = MinAzimuth; azimuth < MaxAzimuth; azimuth += 5.0f)
	{
		for (float inclination = MinInclination; inclination < MaxInclination; inclination += 5.0f)
		{
			inc = toRadians(inclination);
			azi = toRadians(azimuth);

			X = fRadius * sinf(inc) * cosf(azi);
			Y = fRadius * sinf(inc) * sinf(azi);
			Z = fRadius * cosf(inc);

			glVertex3f(X, Y, Z);
		}

		inc = toRadians(MaxInclination);
		azi = toRadians(azimuth);

		X = fRadius * sinf(inc) * cosf(azi);
		Y = fRadius * sinf(inc) * sinf(azi);
		Z = fRadius * cosf(inc);

		glVertex3f(X, Y, Z);
	}

	inc = toRadians(MaxInclination);
	azi = toRadians(MaxAzimuth);

	X = MaximumRadius * sinf(inc) * cosf(azi);
	Y = MaximumRadius * sinf(inc) * sinf(azi);
	Z = MaximumRadius * cosf(inc);

	glVertex3f(X, Y, Z);

	glEnd();
}

void SphereEmitter::DebugRender()
{
	glTranslatef(Origin.x, Origin.y, Origin.z);

	RenderSphere(Vector4(0.0f, 0.0f, 1.0f, 1.0f), MinimumRadius);
	RenderSphere(Vector4(1.0f, 0.0f, 0.0f, 1.0f), MaximumRadius);

	glTranslatef(-Origin.x, -Origin.y, -Origin.z);
}
