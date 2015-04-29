////////////////////////////////////////
// PlayState.h
////////////////////////////////////////

#ifndef PLAYSTATE_H_
#define PLAYSTATE_H_

#include "core.h"
#include "GameState.h"
#include "Building.h"
#include "Floor.h"
#include "ClientGame.h"
#include <map>

class PlayState : public GameState
{
public:
	PlayState(GLFWwindow* window);
	void Initialize();

	void Input(ClientGame* Client);
	void Update(ClientGame* Client);
	void Draw();

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseButton(GLFWwindow* window, int button, int action, int mods);
	void MouseMotion(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
private:
	GLFWwindow* window;

	// Window management
	float ratio;
	int WindowHandle;
	int WinX, WinY;

	// Input
	bool LeftDown, MiddleDown, RightDown, LeftDownTwo, BothDown;
	double MouseX, MouseY;

	// Components
	Camera Cam;
	Player* player;
	int pid = -1;
	//player is not stored in this map
	std::map<int, GameObject*> gameObjects;
};

////////////////////////////////////////////////////////////////////////////////

#endif