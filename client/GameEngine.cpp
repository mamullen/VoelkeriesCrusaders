////////////////////////////////////////
// GameEngine.cpp
////////////////////////////////////////

#include "GameEngine.h"

////////////////////////////////////////////////////////////////////////////////

static GameEngine *Game;
static GameState* State;

////////////////////////////////////////////////////////////////////////////////

// glfw call member hacks
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)		{ State->KeyCallback(window, key, scancode, action, mods); }
static void mouse_motion(GLFWwindow* window, double xpos, double ypos)							{ State->MouseMotion(window, xpos, ypos); }
static void mouse_button(GLFWwindow* window, int button, int action, int mods)					{ State->MouseButton(window, button, action, mods); }
static void mouse_scroll(GLFWwindow* window, double xoffset, double yoffset)					{ State->MouseScroll(window, xoffset, yoffset); }
static void error_callback(int error, const char* description)									{ fputs(description, stderr); }

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	Game = new GameEngine(argc, argv);	//initialize

	double currentTime = glfwGetTime();

	while (!glfwWindowShouldClose(Game->Window())) {
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;

		currentTime = newTime;
		//std::cout << currentTime << std::endl;

		//currently goes as fast as possible
		Game->Input();
		Game->Update();
		Game->Draw();
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

GameEngine::GameEngine(int argc, char **argv) {
	m_running = true;

	// pass in glfwGetPrimaryMonitor() to first null for fullscreen
	window = glfwCreateWindow(1024, 768, WINDOWTITLE, NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// Currently just starts it with the PlayState
	PlayState *state = new PlayState(window);
	PushState(state);
	ChangeState(state);
	
	//Callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_motion);
	glfwSetMouseButtonCallback(window, mouse_button);
	glfwSetScrollCallback(window, mouse_scroll);
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::ChangeState(GameState *state) {
	State = state;
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::PushState(GameState *state) {
	states.push(state);
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::PopState() {
	states.pop();
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::Input() {
	State->Input();
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::Update() {
	State->Update();
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::Draw() {
	State->Draw();
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::Quit() {
	m_running = false;
	glFinish();
	glfwDestroyWindow(Game->window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////

GLFWwindow* GameEngine::Window() {
	return window;
}

////////////////////////////////////////////////////////////////////////////////