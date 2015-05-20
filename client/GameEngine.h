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

	void Input(ClientGame* Client);
	void Update(ClientGame* Client);
	void Draw();

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
