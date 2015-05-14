#ifndef PSYS_H_
#define PSYS_H_

#include "core.h"
#include "Shaders.h"
#include "Texture.h"
#include "vector3.h"
#include <algorithm>

const int MaxParticles = 100000;

// CPU representation of a particle
struct Particle{
	Vector3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if < 0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class Particles {
public:
	Particles();
	~Particles();
	void Init();
	int FindUnusedParticle();
	void GenerateNewParticles(float deltaTime);
	void SortParticles();
	void Loop(double delta);

private:
	Particle ParticlesContainer[MaxParticles];
	int LastUsedParticle;

	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

	GLuint VertexArrayID;
	GLuint TextureID;
	GLuint programID;

	GLuint CameraRight_worldspace_ID;
	GLuint CameraUp_worldspace_ID;
	GLuint ViewProjMatrixID;

	GLuint Texture;

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;
};

#endif