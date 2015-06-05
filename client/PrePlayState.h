#pragma once

#include "GameState.h"
#include <GL/glut.h>

//State Constants
const int CHOOSE_CRUSADER = GLFW_KEY_1;
const int CHOOSE_VAMPIRE = GLFW_KEY_2;

class PrePlayState : public GameState
{
public:
	PrePlayState(GLFWwindow* window) :GameState(window){ Initialize(); };

	int Initialize();
	void Input(ClientGame* Client);
	void Update(ClientGame* Client);
	void Draw(ClientGame* client);
	void SetName(char* n)			{ name = n; };

	GLuint LoadRAWTexture(const char * filename, int width, int height);

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void CharCallback(GLFWwindow* window, unsigned int code);
	void MouseButton(GLFWwindow* window, int button, int action, int mods) {};
	void MouseMotion(GLFWwindow* window, double xpos, double ypos) {};
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset) {};
private:
	void drawTitleImage(int i);
	// Window management
	float ratio;
	int WindowHandle;
	int WinX, WinY;

	//PrePlayState variables
	int curr_team = 0;
	int maxCrusaders = 0 , maxVampires = 0;
	bool team_full = false, send_name = false, wait_name = false;
	bool name_state = true, name_error = false;
	std::string name = "";
	GLuint textureID;
	Texture t;
	GLuint photos[1];
	// Components
	Camera Cam;
};

