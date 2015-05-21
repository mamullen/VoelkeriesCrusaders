#include "PrePlayState.h"
#include "GameEngine.h"

int PrePlayState::Initialize()
{
	string configWinX;
	string configWinY;
	ConfigSettings::config->getValue("WinX", configWinX);
	ConfigSettings::config->getValue("WinY", configWinY);
	WinX = stoi(configWinX);
	WinY = stoi(configWinY);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);	// no vsync

	glfwGetFramebufferSize(window, &WinX, &WinY);
	ratio = WinX / (float)WinY;

	// Background color
	glClearColor(0.5, 0., 0., 1.);
	glEnable(GL_DEPTH_TEST);

	// Initialize components
	Cam.SetAspect(float(WinX) / float(WinY));

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//initialize state variables
	curr_team = 0;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::Update(ClientGame* client) {
	Packet * serverPacket = client->popJoinGameEvent();

	while (serverPacket != NULL){
		char * serverEvent = serverPacket->packet_data;
		unsigned int objID = serverPacket->id;

		printf("%s\n", serverEvent);
		//printf("%d\n", objID);

		if (strstr(serverEvent, "new") != NULL){
			client->setClientId(objID);
			printf("Setting ID: %d\n", objID);
		}

		if (strstr(serverEvent, "crusader") != NULL){
			curr_team = 1;
		}

		if (strstr(serverEvent, "vampire") != NULL){
			curr_team = 2;
		}

		if (strstr(serverEvent, "team_full") != NULL){
			curr_team = 3;
		}

		if (strstr(serverEvent, "game_start") != NULL){
			client->setStateChange("play_state");
		}

		delete serverPacket;
		serverPacket = client->popJoinGameEvent();
	}
}

void drawText(const char *text, int x, int y){
	glMatrixMode(GL_PROJECTION); // change the current matrix to PROJECTION
	double matrix[16]; // 16 doubles in stack memory
	glGetDoublev(GL_PROJECTION_MATRIX, matrix); // get the values from PROJECTION matrix to local variable
	glLoadIdentity(); // reset PROJECTION matrix to identity matrix
	glOrtho(0, 800, 0, 600, -5, 5); // orthographic perspective
	glMatrixMode(GL_MODELVIEW); // change current matrix to MODELVIEW matrix again
	glLoadIdentity(); // reset it to identity matrix
	glPushMatrix(); // push current state of MODELVIEW matrix to stack
	glRasterPos2i(x, y); // raster position in 2D
	for (int i = 0; i<strlen(text); i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]); // generation of characters in our text with 9 by 15 GLU font
	}
	glPopMatrix(); // get MODELVIEW matrix value from stack
	glMatrixMode(GL_PROJECTION); // change current matrix mode to PROJECTION
	glLoadMatrixd(matrix); // reset
	glMatrixMode(GL_MODELVIEW); // change current matrix mode to MODELVIEW
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::Draw() {

	// Set up glStuff
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawText("Press 1 to be a Crusader", 150, 570);
	drawText("Press 2 to be a Vampire", 500, 570);

	if (curr_team == 1){
		drawText("You are a Crusader!", 345, 500);
	}
	else if (curr_team == 2){
		drawText("You are a Vampire!", 345, 500);
	}
	else if (curr_team == 3){
		drawText("Team is full... join the other team.", 290, 500);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::Input(ClientGame* client) {

	if (glfwGetKey(window, CHOOSE_CRUSADER)) {
		client->addEvent(0, "choose_crusader;", JOIN_GAME);
	}

	if (glfwGetKey(window, CHOOSE_VAMPIRE)) {
		client->addEvent(0, "choose_vampire;", JOIN_GAME);
	}
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit
		glfwSetWindowShouldClose(window, GL_TRUE);
}