////////////////////////////////////////
// tester.h
////////////////////////////////////////

#ifndef CSE169_TESTER_H
#define CSE169_TESTER_H

#include "core.h"
#include "camera.h"
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
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown,LeftDownTwo;
	int MouseX,MouseY;

	// Components
	Camera Cam;
    bool temp;
    bool n;
    bool s;
    int skelskin;
    int light;
    int dofnum;
    float dofx;
    float dofy;
    float dofz;
    bool cull;
    float tt;
    int posjoint;
    bool rotate;
    float newx,newy,newz;
    int xtimes,ytimes,ztimes;
    int nextx, nexty;
    bool gridon;
    bool floor;
    
};

////////////////////////////////////////////////////////////////////////////////

/*
The 'Tester' is a simple top level application class. It creates and manages a
window with the GLUT extension to OpenGL and it maintains a simple 3D scene
including a camera and some other components.
*/

#endif
