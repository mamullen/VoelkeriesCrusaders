////////////////////////////////////////
// GameEngine.h
////////////////////////////////////////

#ifndef GAMEENG_H_
#define GAMEENG_H

#include "core.h"
#include "GameState.h"
#include "PlayState.h"
#include "PrePlayState.h"
#include "ClientGame.h"
#include <thread>
#include <stack>
#include "objparser.h"
#include "ConfigSettings.h"
// Include file for sound
#include "mmsystem.h"
// Link to this library for sound
#pragma comment( lib, "winmm.lib" ) 


using namespace std;
////////////////////////////////////////////////////////////////////////////////

class GameEngine {
public:
	GameEngine(int argc, char **argv);
	void Initialize();

	// States take care of what the current state the game is in
	void ChangeState(GameState *state);
	void PushState(GameState *state);
	void PopState();
	GameState *PeekState() { return states.top(); }

	void Input(ClientGame* Client);
	void Update(ClientGame* Client);
	void Draw(ClientGame* client);

	bool Running()	{ return m_running; }
	void Quit();

	GLFWwindow* Window();
private:
	GLFWwindow* window;
	bool m_running;
	stack<GameState*> states;
};

////////////////////////////////////////////////////////////////////////////////

#endif
