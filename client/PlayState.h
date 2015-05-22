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

class PlayState : public GameState
{
public:
	PlayState(GLFWwindow* window) :GameState(window) { Initialize(); };
	
	int Initialize();
	int InitGL();

	void Input(ClientGame* Client);
	void Update(ClientGame* Client);

	// Draw Methods
	void Draw();
	void RenderParticles();

	void UpdateParticles();
	void UpdateClient(ClientGame* Client);

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
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
};

////////////////////////////////////////////////////////////////////////////////

#endif