////////////////////////////////////////
// PlayState.h
////////////////////////////////////////

#ifndef PLAYSTATE_H_
#define PLAYSTATE_H_

#include "core.h"
#include "GameState.h"
#include "DaeLoader.h"
#include "Building.h"
#include "Floor.h"

class PlayState : public GameState
{
public:
	PlayState(GLFWwindow* window);
	void Initialize();

	void Input();
	void Update();
	void Draw();

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseButton(GLFWwindow* window, int button, int action, int mods);
	void MouseMotion(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
private:
	GLFWwindow* window;
	Building* b1;
	Building* b2;

	// Window management
	float ratio;
	int WindowHandle;
	int WinX, WinY;

	// Input
	bool LeftDown, MiddleDown, RightDown, LeftDownTwo, BothDown;
	double MouseX, MouseY;

	// Components
	Camera Cam;
	Player player;
	DaeLoader mech;
    Floor field;
};

////////////////////////////////////////////////////////////////////////////////

#endif