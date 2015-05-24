////////////////////////////////////////
// GameEngine.cpp
////////////////////////////////////////

#include "GameEngine.h"

////////////////////////////////////////////////////////////////////////////////

static GameEngine* Game;
static GameState* State;
static ClientGame* Client;

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

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	Game = new GameEngine(argc, argv);	//initialize
	if (Game->PeekState()->Initialize() != 0) {
		fprintf(stderr, "Failed to initialize state\n");
		return -1;
	}

	double currentTime = glfwGetTime();

	while (!glfwWindowShouldClose(Game->Window())) {
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;

		currentTime = newTime;
		//std::cout << frameTime << std::endl;

		//currently goes as fast as possible

		//get Client inputs.  Pass Client as param so that we can add events to
		//the Client to later send to server if we need to
		Game->Input(Client);
		//send Client inputs to server and receive messages from server, if any
		Client->update();
		//update and render game based off of Server messages
		Game->Update(Client);
		Game->Draw();

		//check if client wants to change state
		if (Client->getStateChange() != NULL){
			if (strcmp(Client->getStateChange(), "play_state") == 0){
				Game->ChangeState(new PlayState(Game->Window()));
			}
			Client->setStateChange(NULL);
		}
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
	//PlayState *state = new PlayState(window);
	PrePlayState *state = new PrePlayState(window);
	PushState(state);
	ChangeState(state);
	
	// Initialize the part of the client that connects to the server
	Client = new ClientGame();
	//string configIP;
	//ConfigSettings::config->getValue("ServerIP", configIP);
	//printf("Connecting to Server IP: %s\n", configIP.c_str());
	Client->connectToServer(ConfigSettings::config->getValue("ServerIP").c_str()); //set ip of server here

	//Callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_motion);
	glfwSetMouseButtonCallback(window, mouse_button);
	glfwSetScrollCallback(window, mouse_scroll);
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::ChangeState(GameState* state) {
	State = state;
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::PushState(GameState* state) {
	states.push(state);
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::PopState() {
	states.pop();
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::Input(ClientGame* client) {
	State->Input(client);
}

////////////////////////////////////////////////////////////////////////////////

void GameEngine::Update(ClientGame* client) {
	State->Update(client);
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