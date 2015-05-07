////////////////////////////////////////
// tester.h
////////////////////////////////////////

#ifndef CSE169_TESTER_H
#define CSE169_TESTER_H

#include "core.h"
#include "GameState.h"
#include "PlayState.h"
#include <thread>
#include "objparser.h"


using namespace std;
////////////////////////////////////////////////////////////////////////////////

class Tester {
public:
	GLFWwindow* window;
	Tester(int argc,char **argv);
	~Tester();

	void ChangeState(GameState *state);
	void PushState(GameState *state);

	void PopState();

	void Initialize();
	void Cleanup();
	void Input();
	void Update();
	void Draw();

	bool Running()	{ return m_running; }
	void Quit();

private:
	bool m_running;
	vector<GameState> states;
};

////////////////////////////////////////////////////////////////////////////////

/*
The 'Tester' is a simple top level application class. It creates and manages a
window with the GLUT extension to OpenGL and it maintains a simple 3D scene
including a camera and some other components.
*/

#endif
