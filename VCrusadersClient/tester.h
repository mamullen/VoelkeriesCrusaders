////////////////////////////////////////
// tester.h
////////////////////////////////////////

#ifndef CSE169_TESTER_H
#define CSE169_TESTER_H

#include "core.h"
#include "camera.h"
#include "player.h"
#include "objparser.h"
#include "GamePlay.h"
#include <vector>

using namespace std;
////////////////////////////////////////////////////////////////////////////////

class Tester {
public:
	Tester(int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Loop();
	void UpdateFromServer();

	void Quit();

	// Event handlers
	void Resize(int x, int y);	
	void MouseButton(GLFWwindow* window, int button, int action, int mods);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	void MouseMotion(GLFWwindow* window, double xpos, double ypos);

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	// Window management
	float ratio;
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown,LeftDownTwo,BothDown;
	int MouseX,MouseY;

	// Components
	Camera Cam;
	Player player;
	bool rotate; 



};

// Backend server business
GamePlay * gameplay;



////////////////////////////////////////////////////////////////////////////////

/*
The 'Tester' is a simple top level application class. It creates and manages a
window with the GLUT extension to OpenGL and it maintains a simple 3D scene
including a camera and some other components.
*/

#endif
