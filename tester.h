////////////////////////////////////////
// tester.h
////////////////////////////////////////

#ifndef CSE169_TESTER_H
#define CSE169_TESTER_H

#include "core.h"
#include "camera.h"
#include "player.h"
#include <iostream>
using namespace std;
////////////////////////////////////////////////////////////////////////////////

class Tester {
public:
	Tester(int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x, int y);	
	void KeyboardDown(int key, int x, int y);
	void KeyboardUp(int key, int x, int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseRoll(int wheel, int dir, int x, int y);
	void MouseMotion(int x,int y);
	void KeyOperations();

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown,LeftDownTwo,BothDown;
	int MouseX,MouseY;

	// Components
	Camera Cam;
	Player player;
	bool rotate;
	unsigned char keys[256];
};

////////////////////////////////////////////////////////////////////////////////

/*
The 'Tester' is a simple top level application class. It creates and manages a
window with the GLUT extension to OpenGL and it maintains a simple 3D scene
including a camera and some other components.
*/

#endif
