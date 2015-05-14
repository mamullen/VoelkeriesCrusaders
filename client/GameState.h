#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "core.h"
#include "player.h"
#include "camera.h"
#include "ClientGame.h"

class GameState {
public:
	virtual int Initialize() = 0;
	
	/*virtual void Pause() = 0;
	virtual void Resume() = 0;*/

	virtual void Input(ClientGame* Client) = 0;
	virtual void Update(ClientGame* Client) = 0;
	virtual void Draw() = 0;

	virtual void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
	virtual void MouseButton(GLFWwindow* window, int button, int action, int mods) = 0;
	virtual void MouseMotion(GLFWwindow* window, double xpos, double ypos) = 0;
	virtual void MouseScroll(GLFWwindow* window, double xoffset, double yoffset) = 0;
protected:
	GameState() {}
};

#endif