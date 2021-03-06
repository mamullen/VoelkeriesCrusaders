////////////////////////////////////////
// GameEngine.h
////////////////////////////////////////

#ifndef GAMEENG_H_
#define GAMEENG_H

#include "core.h"
#include "GameState.h"
#include "PlayState.h"
#include <thread>
#include <stack>
#include "objparser.h"

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

	void Input();
	void Update();
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
