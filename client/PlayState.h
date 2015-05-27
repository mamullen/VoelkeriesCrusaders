////////////////////////////////////////
// PlayState.h
////////////////////////////////////////

#ifndef PLAYSTATE_H_
#define PLAYSTATE_H_

#include "core.h"
#include "ElapsedTime.h"
#include "ParticleSystem.h"
#include "Emitter.h"
#include "GameState.h"
#include "Building.h"
#include "Floor.h"
#include "ClientGame.h"
#include <map>
#include "Shader.h"
#include "Texture.h"
#if ( (defined(__MACH__)) && (defined(__APPLE__)) )
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "Shrine.h"

//#include <fbxsdk/fbxsdk_def.h>




class PlayState : public GameState
{
public:
	PlayState(GLFWwindow* window) :GameState(window) { Initialize(); };
	
	int Initialize();
	int InitGL();

	void Input(ClientGame* Client);
	void Update(ClientGame* Client);

	// Draw Methods
	void Draw(ClientGame* client);
	void RenderParticles(float rot);

	void UpdateParticles();
	void UpdateClient(ClientGame* Client);

	void drawsomeground();

	GLuint LoadRAWTexture(const char * filename, int width, int height);

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void CharCallback(GLFWwindow* window, unsigned int code);
	void MouseButton(GLFWwindow* window, int button, int action, int mods);
	void MouseMotion(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
private:
	// Window management
	float ratio;
	int WindowHandle;
	int WinX, WinY;

	// Input
	bool LeftDown, MiddleDown, RightDown, LeftDownTwo, BothDown;
	bool rotationChanged, attacking;
	double MouseX, MouseY;
	double lastTime;

	// Components
	Camera Cam;
	Player* player;

	SphereEmitter g_ParticleEmitter;
	CubeEmitter g_CubeEmitter;

	int pid = -1;
	//player is not stored in this map
	std::map<int, GameObject*> gameObjects;
	Building* b1;
	Texture t;
	GLuint photos[5];
	Shader shader;
	//Shader floorshader;
	GLuint text_picture, text_normalmap;
	GLuint floor_picture, floor_normalmap;
	GLuint skyfront_picture, skyfront_normalmap, skyback_picture, skyback_normalmap, skyleft_picture, skyleft_normalmap, skyright_picture, skyright_normalmap, skytop_picture, skytop_normalmap;
	bool once;
	
};

////////////////////////////////////////////////////////////////////////////////

#endif